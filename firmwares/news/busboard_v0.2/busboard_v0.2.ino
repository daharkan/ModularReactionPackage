/*
  Busboard (ATmega328PB / MiniCore) - 5-slot SoftwareSerial mux + local flow/temp + buzzer
  PC command format: "<slot>-<cmd>"  e.g. "1-p30"

  Local (busboard) commands (slot 0):
    0-flow
    0-temp
    0-sensors
    0-flowreset
    0-sensors_scont     (continuous; stops when any new PC cmd arrives)
    0-sensors_cont      (alias)
    0-buzz200           (optional: beep 200ms)

  Notes:
  - Flow pulse is on PE2 (Arduino D25 / A6). We do NOT use PCINT or attachInterrupt.
    We sample PE2 with Timer2 COMPA interrupt (2kHz) to avoid ISR vector clashes with SoftwareSerial.
  - Temp is on A3 (Arduino 17). TEMP_VREF is used only for mV conversion.
*/

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <avr/interrupt.h>

// ---------- Config ----------
static const uint32_t PC_BAUD     = 19200;
static const uint32_t SLAVE_BAUD  = 9600;

static const bool INTERLOCK_ACTIVE_LOW = false;
static const bool SLAVE_APPEND_NEWLINE = true;

static const uint16_t CMD_RESPONSE_MS = 300;
static const uint16_t RR_LISTEN_US = 1500;

static const uint16_t SENSORS_STREAM_PERIOD_MS = 500;

// If you feed 2.5V to AREF and use analogReference(EXTERNAL), set this to 2.5f
static const float TEMP_VREF = 5.0f;

// Flow conversion (K-factor)
static const float FLOW_PULSES_PER_LITER = 450.0f; // TODO: calibrate
static const uint16_t FLOW_CALC_PERIOD_MS = 500;

// ---------- Pins ----------
// Slot UART pins
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

// Interlocks (kept as you had them; adjust if your new PCB differs)
static const uint8_t PIN_IL1 = 26; // read via PE3 register below
static const uint8_t PIN_IL2 = 19; // A5
static const uint8_t PIN_IL3 = 18; // A4
static const uint8_t PIN_IL4 = 16; // A2
static const uint8_t PIN_IL5 = 14; // A0

// Sensors
static const uint8_t PIN_FLOW_TEMP  = 17; // A3
static const uint8_t PIN_FLOW_PULSE = 25; // PE2 / D25 / A6 (FLOW input)

// Buzzer moved to some other pin? If you really want buzzer on PE2 too, you can't share it with flow.
// So set buzzer pin to wherever your buzzer is now. If buzzer is on PE2 and flow is also PE2 -> not possible.
static const uint8_t PIN_BUZZER = 25; // <-- CHANGE if buzzer is not on PE2

// ---------- SoftwareSerial instances ----------
SoftwareSerial ss1(PIN_S1_RX, PIN_S1_TX);
SoftwareSerial ss2(PIN_S2_RX, PIN_S2_TX);
SoftwareSerial ss3(PIN_S3_RX, PIN_S3_TX);
SoftwareSerial ss4(PIN_S4_RX, PIN_S4_TX);
SoftwareSerial ss5(PIN_S5_RX, PIN_S5_TX);

static SoftwareSerial* const SSS[5] = { &ss1, &ss2, &ss3, &ss4, &ss5 };
static const uint8_t IL_PINS[5]     = { PIN_IL1, PIN_IL2, PIN_IL3, PIN_IL4, PIN_IL5 };

// ---------- State ----------
static bool present[5]     = { false, false, false, false, false };
static bool presentPrev[5] = { false, false, false, false, false };

static uint8_t rrIndex = 0;

// per-slot line buffers
static char lineBuf[5][128];
static uint8_t lineLen[5] = {0,0,0,0,0};

// PC RX buffer (line-based)
static char pcBuf[128];
static uint8_t pcLen = 0;

// Flow measurement
static volatile uint32_t g_flowPulses = 0;
static volatile uint8_t  g_flowPrev   = 0;

static uint32_t flow_lastCalcMs = 0;
static uint32_t flow_lastPulses = 0;
static float flow_hz = 0.0f;
static float flow_lpm = 0.0f;

// Continuous stream
static bool sensors_stream = false;
static uint32_t sensors_lastPrintMs = 0;

// ---------- Interlock helpers ----------
static inline bool readInterlock1Present() {
  // Interlock1 on PE3 (as per your earlier code)
  return (PINE & (1 << PE3)) != 0;
}

static inline bool readInterlockPresent(uint8_t pin) {
  int v = digitalRead(pin);
  if (INTERLOCK_ACTIVE_LOW) return (v == LOW);
  return (v == HIGH);
}

static void logPresenceChange(uint8_t slot1based, bool isPresent) {
  Serial.print(F("slot "));
  Serial.print(slot1based);
  Serial.print(F(" "));
  Serial.println(isPresent ? F("connected") : F("disconnected"));
}

// ---------- Slave line handling ----------
static void flushLine(uint8_t slotIdx) {
  if (lineLen[slotIdx] == 0) return;
  lineBuf[slotIdx][lineLen[slotIdx]] = '\0';

  Serial.print(F("slave"));
  Serial.print(slotIdx + 1);
  Serial.print(F(" says: "));
  Serial.println(lineBuf[slotIdx]);

  lineLen[slotIdx] = 0;
}

static void feedCharToLine(uint8_t slotIdx, char c) {
  if (c == '\r') return;
  if (c == '\n') { flushLine(slotIdx); return; }

  if (lineLen[slotIdx] < sizeof(lineBuf[slotIdx]) - 1) {
    lineBuf[slotIdx][lineLen[slotIdx]++] = c;
  } else {
    flushLine(slotIdx);
    lineBuf[slotIdx][lineLen[slotIdx]++] = c;
  }
}

static void drainActiveSlave(uint8_t slotIdx, uint32_t durationMs) {
  SoftwareSerial* ss = SSS[slotIdx];
  ss->listen();

  uint32_t t0 = millis();
  while ((uint32_t)(millis() - t0) < durationMs) {
    while (ss->available() > 0) {
      char c = (char)ss->read();
      feedCharToLine(slotIdx, c);
    }
    delay(1);
  }
}

static void rrPollOnce() {
  for (uint8_t k = 0; k < 5; k++) {
    uint8_t i = (rrIndex + k) % 5;

    rrIndex = (i + 1) % 5;
    SoftwareSerial* ss = SSS[i];
    ss->listen();

    uint32_t tStart = micros();
    while ((uint32_t)(micros() - tStart) < RR_LISTEN_US) {
      while (ss->available() > 0) {
        char c = (char)ss->read();
        feedCharToLine(i, c);
      }
    }
    return;
  }
  rrIndex = (rrIndex + 1) % 5;
}

// ---------- Flow sampling via Timer2 (no PCINT / no attachInterrupt) ----------
ISR(TIMER2_COMPA_vect) {
  uint8_t now = (PINE & (1 << PE2)) ? 1 : 0;   // FLOW on PE2 (D25/A6)
  if (now && !g_flowPrev) g_flowPulses++;      // rising only
  g_flowPrev = now;
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

static uint16_t flow_temp_adc() {
  return (uint16_t)analogRead(PIN_FLOW_TEMP);
}

static void print_flow() {
  uint32_t pulses;
  noInterrupts();
  pulses = g_flowPulses;
  interrupts();

  Serial.print(F("flow_pulses=")); Serial.print(pulses);
  Serial.print(F(" hz="));         Serial.print(flow_hz, 2);
  Serial.print(F(" lpm="));        Serial.println(flow_lpm, 3);
}

static void print_temp() {
  uint16_t adc = flow_temp_adc();
  float mv = (adc * (TEMP_VREF * 1000.0f)) / 1023.0f;

  Serial.print(F("flow_temp_adc=")); Serial.print(adc);
  Serial.print(F(" mv="));           Serial.println(mv, 1);
}

static void flow_reset() {
  noInterrupts();
  g_flowPulses = 0;
  interrupts();

  flow_lastPulses = 0;
  flow_lastCalcMs = millis();
  flow_hz = 0.0f;
  flow_lpm = 0.0f;

  Serial.println(F("flow_reset=ok"));
}

static void sensors_stream_tick() {
  if (!sensors_stream) return;

  uint32_t now = millis();
  if ((uint32_t)(now - sensors_lastPrintMs) < SENSORS_STREAM_PERIOD_MS) return;

  sensors_lastPrintMs = now;
  print_flow();
  print_temp();
}

// ---------- Buzzer ----------
static inline void buzz(int mSecs) {
  // WARNING: If buzzer shares the same PE2 pin with flow input, you cannot use both.
  // If PIN_BUZZER != 25, this will use Arduino API safely.
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, HIGH);
  delay(mSecs);
  digitalWrite(PIN_BUZZER, LOW);
}

// ---------- Interlocks ----------
static void pollInterlocks() {
  for (uint8_t i = 1; i < 5; i++) {
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

// ---------- PC command handling ----------
static void sendCommandToSlot(uint8_t slot1based, const char* cmd) {
  if (slot1based < 1 || slot1based > 5) {
    Serial.println(F("ERR: slot must be 1..5"));
    return;
  }

  uint8_t idx = slot1based - 1;
  SoftwareSerial* ss = SSS[idx];
  ss->listen();

  Serial.print(F("to slave"));
  Serial.print(slot1based);
  Serial.print(F(": "));
  Serial.println(cmd);

  ss->print(cmd);
  if (SLAVE_APPEND_NEWLINE) ss->print('\n');

  drainActiveSlave(idx, CMD_RESPONSE_MS);
}

static void handleLocalCmd(const char* cmd) {
  if (!cmd || !*cmd) { Serial.println(F("ERR: empty local cmd")); return; }

  if (strcmp(cmd, "flow") == 0)      { print_flow(); return; }
  if (strcmp(cmd, "temp") == 0)      { print_temp(); return; }
  if (strcmp(cmd, "sensors") == 0)   { print_flow(); print_temp(); return; }
  if (strcmp(cmd, "flowreset") == 0) { flow_reset(); return; }

  if (strcmp(cmd, "sensors_scont") == 0 || strcmp(cmd, "sensors_cont") == 0) {
    sensors_stream = true;
    sensors_lastPrintMs = 0;
    Serial.println(F("sensors_stream=on"));
    return;
  }

  if (strncmp(cmd, "buzz", 4) == 0) {
    int ms = atoi(cmd + 4);
    if (ms <= 0) ms = 100;
    buzz(ms);
    return;
  }

  Serial.print(F("ERR: unknown local cmd: "));
  Serial.println(cmd);
}

static void handlePcLine(const char* line) {
  sensors_stream = false; // any new cmd stops continuous

  const char* dash = strchr(line, '-');
  if (!dash || dash == line) {
    Serial.println(F("ERR: format must be <slot>-<cmd>  e.g. 1-p30 or 0-flow"));
    return;
  }

  char slotStr[8];
  size_t n = (size_t)(dash - line);
  if (n >= sizeof(slotStr)) n = sizeof(slotStr) - 1;
  memcpy(slotStr, line, n);
  slotStr[n] = '\0';

  int slot = atoi(slotStr);
  const char* payload = dash + 1;
  if (*payload == '\0') {
    Serial.println(F("ERR: empty cmd"));
    return;
  }

  if (slot == 0) { handleLocalCmd(payload); return; }
  sendCommandToSlot((uint8_t)slot, payload);
}

static void pollPcSerial() {
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == '\r') continue;

    if (c == '\n') {
      pcBuf[pcLen] = '\0';
      if (pcLen > 0) handlePcLine(pcBuf);
      pcLen = 0;
      continue;
    }

    if (pcLen < sizeof(pcBuf) - 1) {
      pcBuf[pcLen++] = c;
    } else {
      pcLen = 0;
      Serial.println(F("ERR: PC line too long"));
    }
  }
}

// ---------- Setup/Loop ----------
void setup() {
  Serial.begin(PC_BAUD);

  // Interlock 2..5 digital
  for (uint8_t i = 1; i < 5; i++) {
    pinMode(IL_PINS[i], INPUT_PULLUP);
    present[i] = presentPrev[i] = readInterlockPresent(IL_PINS[i]);
  }

  // Interlock 1 on PE3 (register)
  DDRE  &= ~(1 << PE3);
  PORTE |=  (1 << PE3);
  present[0] = presentPrev[0] = readInterlock1Present();

  // SoftwareSerial ports
  ss1.begin(SLAVE_BAUD);
  ss2.begin(SLAVE_BAUD);
  ss3.begin(SLAVE_BAUD);
  ss4.begin(SLAVE_BAUD);
  ss5.begin(SLAVE_BAUD);

  // Temp pin
  pinMode(PIN_FLOW_TEMP, INPUT);

  // Flow input on PE2: input + pullup
  DDRE  &= ~(1 << PE2);
  PORTE |=  (1 << PE2);
  g_flowPrev = (PINE & (1 << PE2)) ? 1 : 0;

  // Timer2: 2kHz sampling interrupt (16MHz / 64 / 125 = 2000Hz)
  TCCR2A = (1 << WGM21);    // CTC
  TCCR2B = (1 << CS22);     // prescaler 64
  OCR2A  = 124;             // 2kHz
  TIMSK2 |= (1 << OCIE2A);  // enable compare A interrupt

  flow_reset();

  Serial.println(F("busboard up"));
  for (uint8_t i = 0; i < 5; i++) logPresenceChange(i + 1, present[i]);
  Serial.println(F("local cmds: 0-flow / 0-temp / 0-sensors / 0-flowreset / 0-sensors_scont / 0-buzzNNN"));
}

void loop() {
  pollInterlocks();
  pollPcSerial();
  rrPollOnce();

  flow_update();
  sensors_stream_tick();
}
