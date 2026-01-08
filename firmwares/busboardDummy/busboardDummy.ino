#include <Arduino.h>

#ifndef IRAM_ATTR
#define IRAM_ATTR
#endif

#define BAUDRATE 115200
#define SLOT_COUNT 3

static const uint16_t STATUS_PERIOD_MS = 500;
static const uint16_t GO_INTERVAL_MS = 200;

static const float TEMP_APPROACH_FACTOR = 0.15f;
static const float TEMP_NOISE_RANGE = 0.6f;

struct CellState {
  float targetTemp;
  float currentTemp;
  int targetRpm;
  int currentRpm;
  float stirrerAmp;
};

static CellState cells[SLOT_COUNT];
static const char* CELL_IDS[SLOT_COUNT] = {
  "s25_111",
  "s25_222",
  "s25_333"
};

static uint32_t lastStatusMs = 0;
static uint32_t lastGoMs = 0;

static float flowLpm = 0.0f;
static float flowTempMv = 0.0f;

static bool parseUpdateCommand(const char* line, int* positionIdx, float* targetTemp, int* targetRpm) {
  if (!line || line[0] == '\0') return false;
  if (line[0] != '>' || line[strlen(line) - 1] != '<') return false;

  char payload[96];
  strncpy(payload, line + 1, sizeof(payload));
  payload[sizeof(payload) - 1] = '\0';
  size_t payloadLen = strlen(payload);
  if (payloadLen == 0) return false;
  payload[payloadLen - 1] = '\0';

  char* token = strtok(payload, "#");
  if (!token) return false;
  int pos = atoi(token);

  token = strtok(nullptr, "#");
  if (!token) return false;
  float tTemp = atof(token);

  token = strtok(nullptr, "#");
  if (!token) return false;
  int tRpm = atoi(token);

  token = strtok(nullptr, "#");
  if (!token) return false;
  int checksumRec = atoi(token);

  int checksumCalc = pos + (int)tTemp + tRpm;
  if (checksumRec != checksumCalc) return false;

  *positionIdx = pos;
  *targetTemp = tTemp;
  *targetRpm = tRpm;
  return true;
}

static void handleSerialUpdates() {
  if (!Serial.available()) return;

  static char lineBuf[128];
  size_t len = Serial.readBytesUntil('\n', lineBuf, sizeof(lineBuf) - 1);
  lineBuf[len] = '\0';
  if (len == 0) return;

  char* start = strchr(lineBuf, '>');
  while (start) {
    char* end = strchr(start, '<');
    if (!end) break;

    char payload[96];
    size_t n = (size_t)(end - start + 1);
    if (n >= sizeof(payload)) {
      start = strchr(end + 1, '>');
      continue;
    }

    strncpy(payload, start, n);
    payload[n] = '\0';

    int positionIdx = -1;
    float targetTemp = 0.0f;
    int targetRpm = 0;

    if (parseUpdateCommand(payload, &positionIdx, &targetTemp, &targetRpm)) {
      if (positionIdx >= 1 && positionIdx <= SLOT_COUNT) {
        int idx = positionIdx - 1;
        cells[idx].targetTemp = targetTemp;
        cells[idx].targetRpm = targetRpm;
      }
    }

    start = strchr(end + 1, '>');
  }
}

static float randomNoise(float amplitude) {
  long raw = random(-1000, 1001);
  return (raw / 1000.0f) * amplitude;
}

static void updateCellTemps() {
  for (int i = 0; i < SLOT_COUNT; i++) {
    float delta = cells[i].targetTemp - cells[i].currentTemp;
    cells[i].currentTemp += delta * TEMP_APPROACH_FACTOR;
    cells[i].currentTemp += randomNoise(TEMP_NOISE_RANGE);

    cells[i].currentRpm = cells[i].targetRpm;
    cells[i].stirrerAmp = 18.0f + randomNoise(2.0f);
  }
}

static void updateFlow() {
  flowLpm += randomNoise(0.2f);
  if (flowLpm < 0.0f) flowLpm = 0.0f;
  if (flowLpm > 3.0f) flowLpm = 3.0f;

  flowTempMv = 250.0f + randomNoise(10.0f);
}

static void sendStatus() {
  char innerStr[12];
  char extStr[12];
  char ampStr[12];
  char targetTempStr[12];
  char flowStr[12];
  char flowTempStr[12];

  for (int i = 0; i < SLOT_COUNT; i++) {
    float innerTemp = cells[i].currentTemp;
    float extTemp = innerTemp + randomNoise(0.4f);

    dtostrf(innerTemp, 4, 1, innerStr);
    dtostrf(extTemp, 4, 1, extStr);
    dtostrf(cells[i].stirrerAmp, 4, 1, ampStr);
    dtostrf(cells[i].targetTemp, 4, 1, targetTempStr);
    dtostrf(flowLpm, 4, 3, flowStr);
    dtostrf(flowTempMv, 4, 1, flowTempStr);

    Serial.print(F("bb_000#"));
    Serial.print(CELL_IDS[i]);
    Serial.print('#');
    Serial.print(i + 1);
    Serial.print('#');
    Serial.print(innerStr);
    Serial.print('#');
    Serial.print(extStr);
    Serial.print('#');
    Serial.print(cells[i].currentRpm);
    Serial.print('#');
    Serial.print(ampStr);
    Serial.print('#');
    Serial.print(targetTempStr);
    Serial.print('#');
    Serial.print(cells[i].targetRpm);
    Serial.print('#');
    Serial.print(flowStr);
    Serial.print('#');
    Serial.println(flowTempStr);
  }
}

static void sendPresenceStatus() {
  for (int i = 0; i < SLOT_COUNT; i++) {
    Serial.print(F("presence#"));
    Serial.print(i + 1);
    Serial.print(F("#"));
    Serial.println(F("1"));
  }
}

void setup() {
  Serial.begin(BAUDRATE);
  randomSeed(analogRead(A0));

  for (int i = 0; i < SLOT_COUNT; i++) {
    cells[i].targetTemp = 25.0f + (i * 2.0f);
    cells[i].currentTemp = cells[i].targetTemp - 3.0f;
    cells[i].targetRpm = 200 + (i * 10);
    cells[i].currentRpm = cells[i].targetRpm;
    cells[i].stirrerAmp = 18.0f;
  }

  flowLpm = 1.2f;
  flowTempMv = 250.0f;

  Serial.println(F("bb_000#HELLO"));
  sendPresenceStatus();
  lastStatusMs = millis();
  lastGoMs = millis();
}

void loop() {
  handleSerialUpdates();

  uint32_t now = millis();
  if ((uint32_t)(now - lastStatusMs) >= STATUS_PERIOD_MS) {
    updateCellTemps();
    updateFlow();
    sendStatus();
    lastStatusMs = now;
  }

  if ((uint32_t)(now - lastGoMs) >= GO_INTERVAL_MS) {
    Serial.println(F("GO"));
    lastGoMs = now;
  }
}
