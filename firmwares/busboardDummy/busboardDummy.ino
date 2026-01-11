#include <Arduino.h>
#include <ctype.h>

#ifndef IRAM_ATTR
#define IRAM_ATTR
#endif

#define BAUDRATE 115200
#define SLOT_COUNT 5

static const uint16_t STATUS_PERIOD_MS = 500;
static const uint16_t GO_INTERVAL_MS = 100;

static const float TEMP_APPROACH_FACTOR = 0.95f;
static const float TEMP_NOISE_RANGE = 0.2f;

struct CellState {
  float targetTemp;
  float currentTemp;
  int targetRpm;
  int currentRpm;
  float stirrerAmp;
  uint8_t motorSelect;
};

static CellState cells[SLOT_COUNT];
static const char* CELL_IDS[SLOT_COUNT] = {
  "s25_111",
  "s25_222",
  "s25_333",
  "s25_444",
  "s25_555"
};

static uint32_t lastStatusMs = 0;
static uint32_t lastGoMs = 0;

static float flowLpm = 0.0f;
static float flowTempMv = 0.0f;

static char* trimLine(char* line) {
  if (!line) return line;
  while (*line && isspace(static_cast<unsigned char>(*line))) {
    line++;
  }
  size_t len = strlen(line);
  while (len > 0 && isspace(static_cast<unsigned char>(line[len - 1]))) {
    line[len - 1] = '\0';
    len--;
  }
  return line;
}

static bool parseUpdateCommand(char* line, int* positionIdx, float* targetTemp, int* targetRpm, uint8_t* motorSelect) {
  if (!line || line[0] == '\0') return false;
  line = trimLine(line);
  if (line[0] == '\0') return false;
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
  int tMotorSelect = atoi(token);

  token = strtok(nullptr, "#");
  if (!token) return false;
  int checksumRec = atoi(token);

  int checksumCalc = pos + (int)tTemp + tRpm + tMotorSelect;
  if (checksumRec != checksumCalc) return false;

  *positionIdx = pos;
  *targetTemp = tTemp;
  *targetRpm = tRpm;
  *motorSelect = (uint8_t)tMotorSelect;
  return true;
}

static bool feedParser(char c, char* out, size_t outSize) {
  static bool inFrame = false;
  static size_t n = 0;

  if (c == '>') { inFrame = true; n = 0; out[n++] = '>'; return false; }
  if (!inFrame) return false;

  if (n < outSize - 1) out[n++] = c;
  else { inFrame = false; n = 0; return false; } // overflow -> drop frame

  if (c == '<') { out[n] = '\0'; inFrame = false; return true; }
  return false;
}

static void handleSerialUpdates() {
  static char frame[96];
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (feedParser(c, frame, sizeof(frame))) {
      int positionIdx; float targetTemp; int targetRpm; uint8_t motorSelect;
      if (parseUpdateCommand(frame, &positionIdx, &targetTemp, &targetRpm, &motorSelect)) {
        if (positionIdx >= 1 && positionIdx <= SLOT_COUNT) {
          int idx = positionIdx - 1;
          cells[idx].targetTemp = targetTemp;
          cells[idx].targetRpm  = targetRpm;
          cells[idx].motorSelect = motorSelect;
        }
      }
    }
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

    Serial.print(F("bbb_RHS_000#"));
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
    cells[i].motorSelect = 0;
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
    Serial.println(F("GO"));
    lastStatusMs = now;
  }


  if ((uint32_t)(now - lastGoMs) >= GO_INTERVAL_MS) {
    Serial.println(F("GO"));
    lastGoMs = now;
  }

}
