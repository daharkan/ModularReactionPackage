#include <Arduino.h>
#include <SoftwareSerial.h>

static const uint32_t PC_BAUD = 19200;
static const uint32_t SLAVE_BAUD = 9600;
static const uint32_t RR_LISTEN_US = 100000; 
static const uint16_t GO_INTERVAL_MS = 200;
static const uint16_t POLL_LIMIT_MS = 250; // Slave'i sürekli sorgulayıp yormamak için
static const size_t LINE_BUF_SIZE = 160; 
#define MACHINE_ID "bbb_LHS_001"

static const uint8_t PIN_IL_PINS[5] = { 26, 19, 18, 16, 14 };
SoftwareSerial ss1(2, 3); SoftwareSerial ss2(4, 5); SoftwareSerial ss3(6, 7);
SoftwareSerial ss4(8, 9); SoftwareSerial ss5(10, 23);
static SoftwareSerial* const SSS[5] = { &ss1, &ss2, &ss3, &ss4, &ss5 };

static char lineBuf[5][LINE_BUF_SIZE];
static uint8_t lineLen[5] = {0};
static char pcBuf[LINE_BUF_SIZE];
static uint8_t pcLen = 0;
static char sharedCopy[LINE_BUF_SIZE];

static bool present[5] = { false };
static bool presentPrev[5] = { false };
static uint8_t rrIndex = 0;
static uint32_t lastGoMs = 0, lastPollMs = 0;

static inline bool readInterlockPresent(uint8_t idx) {
  if (idx == 0) return (PINE & (1 << PE3)) != 0;
  return digitalRead(PIN_IL_PINS[idx]) == HIGH;
}

static int calculateChecksum(const char* m) {
  int c = 0; while(*m) c += *m++; return c;
}

static bool extractChecksum(const char* framed, int* checksumOut) {
  if (!framed || !checksumOut) return false;
  size_t len = strlen(framed);
  if (len < 3 || framed[0] != '>' || framed[len - 1] != '<') return false;
  const char* lastHash = strrchr(framed, '#');
  if (!lastHash || lastHash >= framed + len - 1) return false;
  *checksumOut = atoi(lastHash + 1);
  return true;
}

static bool buildUpdateForSlave(const char* in, char* out, size_t outSize) {
  size_t len = strlen(in);
  if (len < 3 || in[0] != '>' || in[len - 1] != '<') return false;

  if (len + 1 > outSize) return false;

  char payload[LINE_BUF_SIZE];
  size_t payloadLen = len - 2;
  if (payloadLen >= sizeof(payload)) return false;
  memcpy(payload, in + 1, payloadLen);
  payload[payloadLen] = '\0';

  uint8_t hashCount = 0;
  for (size_t i = 0; i < payloadLen; i++) {
    if (payload[i] == '#') hashCount++;
  }

  if (hashCount == 5) {
    strcpy(out, in);
    return true;
  }

  if (hashCount != 4) return false;

  char* tok0 = strtok(payload, "#");
  char* tok1 = strtok(nullptr, "#");
  char* tok2 = strtok(nullptr, "#");
  char* tok3 = strtok(nullptr, "#");
  char* tok4 = strtok(nullptr, "#");
  if (!tok0 || !tok1 || !tok2 || !tok3 || !tok4) return false;

  int pos = atoi(tok0);
  float temp = atof(tok1);
  int rpm = atoi(tok2);
  int motor = atoi(tok3);

  int oldChecksum = pos + (int)temp + rpm + motor;
  int lastVal = atoi(tok4);
  const char* futureStr = (lastVal == oldChecksum) ? tok1 : tok4;
  float future = atof(futureStr);
  int newChecksum = pos + (int)temp + rpm + motor + (int)future;

  snprintf(out, outSize, ">%d#%s#%d#%d#%s#%d<",
           pos, tok1, rpm, motor, futureStr, newChecksum);
  return true;
}

void flushLine(uint8_t slotIdx) {
  if (lineLen[slotIdx] == 0) return;
  lineBuf[slotIdx][lineLen[slotIdx]] = '\0';
  strncpy(sharedCopy, lineBuf[slotIdx], LINE_BUF_SIZE);
  lineLen[slotIdx] = 0;

  char* token = strtok(sharedCopy, "*");
  if (!token) return;
  char* msgPtr = token;
  token = strtok(nullptr, "*");
  if (!token) return;

  if (atoi(token) == calculateChecksum(msgPtr)) {
    char* delim = strchr(msgPtr, '#');
    if (delim) {
      Serial.print(F(MACHINE_ID "#"));
      for(char* p = msgPtr; p < delim; p++) Serial.print(*p);
      Serial.print('#');
      Serial.print(slotIdx + 1);
      Serial.print(delim);
      Serial.println(F("#0.00#0.0"));
    }
  }
}

void rrPollOnce() {
  if (millis() - lastPollMs < POLL_LIMIT_MS) return; // Spam koruması
  
  for (uint8_t k = 0; k < 5; k++) {
    uint8_t i = (rrIndex + k) % 5;
    if (!present[i]) continue;

    rrIndex = (i + 1) % 5;
    lastPollMs = millis();
    SSS[i]->listen();
    SSS[i]->print(F("GO\n")); // Sadece \n gönder
    
    uint32_t tStart = micros();
    while ((uint32_t)(micros() - tStart) < RR_LISTEN_US) {
      if (Serial.available()) return;
      while (SSS[i]->available()) {
        char c = (char)SSS[i]->read();
        if (c == '\r') continue;
        if (c == '\n') { flushLine(i); return; }
        if (lineLen[i] < LINE_BUF_SIZE - 1) lineBuf[i][lineLen[i]++] = c;
      }
    }
    return;
  }
}

void setup() {
  Serial.begin(PC_BAUD);
  for (uint8_t i = 0; i < 5; i++) {
    if (i > 0) pinMode(PIN_IL_PINS[i], INPUT_PULLUP);
    else { DDRE &= ~(1 << PE3); PORTE &= ~(1 << PE3); }
    SSS[i]->begin(SLAVE_BAUD);
  }
}

void loop() {
  for (uint8_t i = 0; i < 5; i++) {
    present[i] = readInterlockPresent(i);
    if (present[i] != presentPrev[i]) {
      presentPrev[i] = present[i];
      Serial.print(F("presence#")); Serial.print(i+1); Serial.print(F("#")); Serial.println(present[i]);
    }
  }

  while (Serial.available()) {
    char c = Serial.read();
    if (c == '>') { pcLen = 0; pcBuf[pcLen++] = c; }
    else if (c == '<' && pcLen > 0) {
      pcBuf[pcLen++] = c; pcBuf[pcLen] = '\0';
      int target = atoi(&pcBuf[1]);
      if (target >= 1 && target <= 5) {
        char outBuf[LINE_BUF_SIZE];
        if (buildUpdateForSlave(pcBuf, outBuf, sizeof(outBuf))) {
          // Debug: show what came from PC and what will be sent to slave
          Serial.print(F("pc_in#"));
          Serial.println(pcBuf);
          Serial.print(F("to_slave#"));
          Serial.println(outBuf);

          SSS[target-1]->listen();
          SSS[target-1]->print(outBuf);
          SSS[target-1]->print('\n'); // Gizli \r eklenmesini engelle

          int checksum = 0;
          if (extractChecksum(outBuf, &checksum)) {
            Serial.print(F("ACK#"));
            Serial.print(target);
            Serial.print('#');
            Serial.println(checksum);
          }
        } else {
          Serial.print(F("pc_bad#"));
          Serial.println(pcBuf);
        }
      } else {
        Serial.print(F("pc_target_bad#"));
        Serial.println(pcBuf);
      }
      pcLen = 0;
    } else if (pcLen < LINE_BUF_SIZE - 1) pcBuf[pcLen++] = c;
  }

  rrPollOnce();

  if (millis() - lastGoMs >= GO_INTERVAL_MS) {
    lastGoMs = millis();
    Serial.print(F(MACHINE_ID "#"));
    for(int i=0; i<5; i++) { Serial.print(present[i]); if(i<4) Serial.print('#'); }
    Serial.println(F("\nGO"));
  }
}
