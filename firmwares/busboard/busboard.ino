#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ctype.h>

#ifndef IRAM_ATTR
  #define IRAM_ATTR
#endif

// ---------- Config ----------
static const uint32_t PC_BAUD = 9600;
static const uint32_t SLAVE_BAUD = 9600;

static const uint16_t RR_LISTEN_US = 1500;
static const uint16_t GO_INTERVAL_MS = 200;
static const bool INTERLOCK_ACTIVE_LOW = false;

// ---------- Pin map (new board) ----------
static const uint8_t PIN_S1_RX = 2;
static const uint8_t PIN_S1_TX = 3;

static const uint8_t PIN_S2_RX = 4;
static const uint8_t PIN_S2_TX = 5;

static const uint8_t PIN_S3_RX = 6;
static const uint8_t PIN_S3_TX = 7;

static const uint8_t PIN_S4_RX = 8;
static const uint8_t PIN_S4_TX = 9;

static const uint8_t PIN_S5_RX = 10;
static const uint8_t PIN_S5_TX = 23;

// ---------- Interlocks ----------
static const uint8_t PIN_IL1 = 26;
static const uint8_t PIN_IL2 = 19;
static const uint8_t PIN_IL3 = 18;
static const uint8_t PIN_IL4 = 16;
static const uint8_t PIN_IL5 = 14;

// ---------- Flow sensor pins ----------
static const uint8_t PIN_FLOW_TEMP  = 17; // A3
static const uint8_t PIN_FLOW_PULSE = 25; // digital 25

// Flow conversion
static const float FLOW_PULSES_PER_LITER = 450.0f; // TODO: calibrate
static const uint16_t FLOW_CALC_PERIOD_MS = 500;

// ---------- SoftwareSerial instances ----------
SoftwareSerial ss1(PIN_S1_RX, PIN_S1_TX);
SoftwareSerial ss2(PIN_S2_RX, PIN_S2_TX);
SoftwareSerial ss3(PIN_S3_RX, PIN_S3_TX);
SoftwareSerial ss4(PIN_S4_RX, PIN_S4_TX);
SoftwareSerial ss5(PIN_S5_RX, PIN_S5_TX);

static SoftwareSerial* const SSS[5] = { &ss1, &ss2, &ss3, &ss4, &ss5 };

static const uint8_t SLOT_COUNT = 5;
static const char BUSBOARD_PREFIX[] = "bb_000#";
static const char BUSBOARD_HELLO[] = "bb_000#HELLO";

static const uint8_t IL_PINS[5] = { PIN_IL1, PIN_IL2, PIN_IL3, PIN_IL4, PIN_IL5 };
static bool present[5] = { false, false, false, false, false };
static bool presentPrev[5] = { false, false, false, false, false };

// per-slot line buffers
static char lineBuf[5][128];
static uint8_t lineLen[5] = {0, 0, 0, 0, 0};

static uint8_t rrIndex = 0;
static uint32_t lastGoMs = 0;

// ---------- Flow measurement state ----------
static volatile uint32_t g_flowPulses = 0;
static uint32_t flow_lastCalcMs = 0;
static uint32_t flow_lastPulses = 0;
static float flow_hz = 0.0f;
static float flow_lpm = 0.0f;

static inline bool readInterlock1Present() {
  return (PINE & (1 << PE3)) != 0;
}

static inline bool readInterlockPresent(uint8_t pin) {
  int v = digitalRead(pin);
  if (INTERLOCK_ACTIVE_LOW) return (v == LOW);
  return (v == HIGH);
}

static void logPresenceChange(uint8_t slot1based, bool isPresent) {
  Serial.print(F("presence#"));
  Serial.print(slot1based);
  Serial.print(F("#"));
  Serial.println(isPresent ? F("1") : F("0"));
}

static void IRAM_ATTR flow_isr() {
  g_flowPulses++;
}

static void flow_update() {
  uint32_t now = millis();
  if ((uint32_t)(now - flow_lastCalcMs) < FLOW_CALC_PERIOD_MS) return;

  uint32_t pulses;
  noInterrupts();
  pulses = g_flowPulses;
  interrupts();

  uint32_t dp = pulses - flow_lastPulses;
  float dt = (now - flow_lastCalcMs) / 1000.0f;

  if (dt > 0.0f) {
    flow_hz = dp / dt;
    flow_lpm = (flow_hz * 60.0f) / FLOW_PULSES_PER_LITER;
  }

  flow_lastPulses = pulses;
  flow_lastCalcMs = now;
}

static float flow_temp_mv() {
  uint16_t adc = (uint16_t)analogRead(PIN_FLOW_TEMP);
  const float vref = 5.0f;
  return (adc * (vref * 1000.0f)) / 1023.0f;
}

static int calculateChecksum(const char* message) {
  int checksum = 0;
  for (int i = 0; message[i] != '\0'; ++i) {
    checksum += message[i];
  }
  return checksum;
}

static bool parseMessageAndCRC(char* input, char* message, int* crc) {
  char* token = strtok(input, "*");
  if (!token) return false;
  strcpy(message, token);
  token = strtok(nullptr, "*");
  if (!token) return false;
  *crc = atoi(token);
  return true;
}

static void flushLine(uint8_t slotIdx) {
  if (lineLen[slotIdx] == 0) return;
  lineBuf[slotIdx][lineLen[slotIdx]] = '\0';

  char msg[128];
  int incomingChecksum = 0;
  int calculatedChecksum = 0;

  char lineCopy[128];
  strncpy(lineCopy, lineBuf[slotIdx], sizeof(lineCopy));
  lineCopy[sizeof(lineCopy) - 1] = '\0';

  if (!parseMessageAndCRC(lineCopy, msg, &incomingChecksum)) {
    lineLen[slotIdx] = 0;
    return;
  }

  calculatedChecksum = calculateChecksum(msg);
  if (incomingChecksum != calculatedChecksum) {
    lineLen[slotIdx] = 0;
    return;
  }

  const char* delimiter = strchr(msg, '#');
  if (!delimiter) {
    lineLen[slotIdx] = 0;
    return;
  }

  char forwardedMsg[128];
  size_t prefixLen = (size_t)(delimiter - msg);
  int wrote = snprintf(forwardedMsg, sizeof(forwardedMsg), "%.*s#%u%s",
                       (int)prefixLen,
                       msg,
                       (unsigned)(slotIdx + 1),
                       delimiter);
  if (wrote < 0 || (size_t)wrote >= sizeof(forwardedMsg)) {
    lineLen[slotIdx] = 0;
    return;
  }

  Serial.print(BUSBOARD_PREFIX);
  Serial.print(forwardedMsg);
  Serial.print('#');
  Serial.print(flow_lpm, 3);
  Serial.print('#');
  Serial.println(flow_temp_mv(), 1);

  lineLen[slotIdx] = 0;
}

static void feedCharToLine(uint8_t slotIdx, char c) {
  if (c == '\r') return;
  if (c == '\n') {
    flushLine(slotIdx);
    return;
  }
  if (lineLen[slotIdx] < sizeof(lineBuf[slotIdx]) - 1) {
    lineBuf[slotIdx][lineLen[slotIdx]++] = c;
  } else {
    flushLine(slotIdx);
    lineBuf[slotIdx][lineLen[slotIdx]++] = c;
  }
}

static void rrPollOnce() {
  for (uint8_t k = 0; k < SLOT_COUNT; k++) {
    uint8_t i = (rrIndex + k) % SLOT_COUNT;
    rrIndex = (i + 1) % SLOT_COUNT;

    SoftwareSerial* ss = SSS[i];
    ss->listen();

    uint32_t tStart = micros();
    while ((uint32_t)(micros() - tStart) < RR_LISTEN_US) {
      while (ss->available() > 0) {
        char c = (char)ss->read();
        feedCharToLine(i, c);
      }
      delay(1);
    }
  }
}

static void pollInterlocks() {
  for (uint8_t i = 1; i < SLOT_COUNT; i++) {
    present[i] = readInterlockPresent(IL_PINS[i]);
    if (present[i] != presentPrev[i]) {
      presentPrev[i] = present[i];
      logPresenceChange(i + 1, present[i]);
      if (!present[i]) flushLine(i);
    }
  }

  present[0] = readInterlock1Present();
  if (present[0] != presentPrev[0]) {
    presentPrev[0] = present[0];
    logPresenceChange(1, present[0]);
    if (!present[0]) flushLine(0);
  }
}

static bool readLineFromPC(char* out, size_t outSize) {
  if (!Serial.available()) return false;
  size_t len = Serial.readBytesUntil('\n', out, outSize - 1);
  out[len] = '\0';
  return len > 0;
}

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

static bool parseUpdateCommand(char* line, int* positionIdx, float* targetTemp, int* targetRpm, int* motorSelect, int* checksumRec) {
  if (!line || line[0] == '\0') return false;
  line = trimLine(line);
  if (line[0] == '\0') return false;
  if (line[0] != '>' || line[strlen(line) - 1] != '<') return false;

  char payload[128];
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
  int checksum = atoi(token);

  int checksumCalc = pos + (int)tTemp + tRpm + tMotorSelect;
  if (checksum != checksumCalc) return false;

  *positionIdx = pos;
  *targetTemp = tTemp;
  *targetRpm = tRpm;
  *motorSelect = tMotorSelect;
  *checksumRec = checksum;
  return true;
}

static void sendUpdateToSlot(uint8_t slotIdx, const char* line) {
  if (slotIdx >= SLOT_COUNT) return;
  SSS[slotIdx]->listen();
  SSS[slotIdx]->print(line);
  SSS[slotIdx]->write('\n');
  SSS[slotIdx]->flush();
}

void setup() {
  Serial.begin(PC_BAUD);

  for (uint8_t i = 1; i < SLOT_COUNT; i++) {
    pinMode(IL_PINS[i], INPUT_PULLUP);
    present[i] = presentPrev[i] = readInterlockPresent(IL_PINS[i]);
  }

  DDRE  &= ~(1 << PE3);
  PORTE |=  (1 << PE3);
  present[0] = presentPrev[0] = readInterlock1Present();

  for (uint8_t i = 0; i < SLOT_COUNT; i++) {
    SSS[i]->begin(SLAVE_BAUD);
  }

  pinMode(PIN_FLOW_PULSE, INPUT_PULLUP);
  pinMode(PIN_FLOW_TEMP, INPUT);

  int irq = digitalPinToInterrupt(PIN_FLOW_PULSE);
  if (irq != NOT_AN_INTERRUPT) {
    attachInterrupt(irq, flow_isr, RISING);
  }
  flow_lastCalcMs = millis();

  Serial.println(BUSBOARD_HELLO);
  for (uint8_t i = 0; i < SLOT_COUNT; i++) {
    logPresenceChange(i + 1, present[i]);
  }
  lastGoMs = millis();
}

void loop() {
  pollInterlocks();
  rrPollOnce();
  flow_update();

  char line[128];
  if (readLineFromPC(line, sizeof(line))) {
    int positionIdx = -1;
    float targetTemp = 0.0f;
    int targetRpm = 0;
    int motorSelect = 0;
    int checksumRec = 0;
    if (parseUpdateCommand(line, &positionIdx, &targetTemp, &targetRpm, &motorSelect, &checksumRec)) {
      if (positionIdx >= 1 && positionIdx <= SLOT_COUNT) {
        sendUpdateToSlot((uint8_t)(positionIdx - 1), line);
        Serial.print(F("ACK#"));
        Serial.print(positionIdx);
        Serial.print(F("#"));
        Serial.println(checksumRec);
      }
    }
  }

  uint32_t now = millis();
  if ((uint32_t)(now - lastGoMs) >= GO_INTERVAL_MS) {
    Serial.println(F("GO"));
    lastGoMs = now;
  }
}
