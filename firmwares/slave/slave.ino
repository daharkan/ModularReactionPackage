#include <Arduino.h>
#include <math.h>
#include <SPI.h>
#include <Adafruit_MAX31865.h>
#include <ctype.h>

// ===================== ID / POSITION =====================
#define CELL_ID "s25_555"

// ===================== RTD =====================
#define RREF 430.0
#define RTD_CS_INNER 2
#define RTD_CS_EXTERNAL 5

// ===================== PELTIER =====================
#define PELTIER_PWM_PIN       4
#define PELTIER_SWITCH_1_PIN  14
#define PELTIER_SWITCH_2_PIN  19
#define PELTIER_EN_PIN        25

// ===================== HEATER =====================
#define HEATER_EN_PIN   10
#define HEATER_PWM_PIN  A4

// ===================== MOTOR =====================
#define MOTOR_SELECT_PIN 7   // LOW: motor 1, HIGH: motor 2
#define MOTOR_DIR_PIN    26
#define MOTOR_PWM_PIN    23
#define MOTOR_FB_PIN      3
#define PULSES_PER_REV    2

#define USB_EN_PIN 24

// ===================== TIMER =====================
#define T1_PERIOD_TICKS 532 // ~15kHz @8MHz, prescaler yok

constexpr uint32_t BAUD = 19200;
constexpr uint16_t RPM_UPDATE_MS = 1000;
constexpr float TEMP_HYSTERESIS = 0.3f;
constexpr float HEAT_K = 8.0f; // duty per degC
constexpr float COOL_K = 8.0f;
constexpr float MOTOR_KP = 0.08f; // pwm step per rpm error
constexpr uint8_t DUTY_MIN = 15; // higher voltage limit (~15V)
constexpr uint8_t DUTY_MAX = 70; // lower voltage limit (~5V)
constexpr float HEAT_ASSIST_THRESHOLD = 6.0f; // degC, enable peltier heating assist
constexpr float PELTIER_HEAT_K = 6.0f;

Adafruit_MAX31865 rtdInner = Adafruit_MAX31865(RTD_CS_INNER);
Adafruit_MAX31865 rtdExternal = Adafruit_MAX31865(RTD_CS_EXTERNAL);

// ===================== PWM STATE =====================
constexpr uint8_t PWM_DUTY_STEPS = 50;

struct FastPin {
  volatile uint8_t* out;
  uint8_t mask;
};

FastPin peltierPwmPin;
FastPin heaterPwmPin;
FastPin motorPwmPin;
volatile bool peltierEnabled = false;
volatile bool heaterEnabled = false;
volatile bool motorEnabled = false;

volatile bool peltierPinHigh = false;
volatile bool heaterPinHigh = false;
volatile bool motorPinHigh = false;

volatile uint16_t peltierOnTicks = 0;
volatile uint16_t peltierOffTicks = T1_PERIOD_TICKS;
volatile uint16_t heaterOnTicks = 0;
volatile uint16_t heaterOffTicks = T1_PERIOD_TICKS;
volatile uint16_t motorOnTicks = 0;
volatile uint16_t motorOffTicks = T1_PERIOD_TICKS;

volatile uint16_t peltierCtr = 0;
volatile uint16_t heaterCtr = 0;
volatile uint16_t motorCtr = 0;

volatile int8_t peltierDutyPercent = 0;
volatile uint8_t heaterDutyPercent = 0;

enum PeltierMode : uint8_t {
  PELTIER_MODE_UNKNOWN = 0,
  PELTIER_MODE_COOLING,
  PELTIER_MODE_HEATING
};

PeltierMode peltierMode = PELTIER_MODE_UNKNOWN;

// ===================== STATE =====================
float currentTempInner = 0.0f;
float currentTempExternal = 0.0f;
bool externalPresent = false;

float targetTemp = 25.0f;
int targetRpm = 0;
uint8_t targetMotorSelect = 0;

volatile unsigned long rpmPulseCount = 0;
unsigned long lastRpmMs = 0;
float currentRpm = 0.0f;

uint8_t motorDutyPercent = 0;


static inline uint16_t dutyToTicks(uint8_t percent) {
  if (percent > 100) percent = 100;
  uint8_t steps = (uint32_t)percent * PWM_DUTY_STEPS / 100u;
  return (uint32_t)steps * T1_PERIOD_TICKS / PWM_DUTY_STEPS;
}

static uint8_t clampDuty(uint8_t duty) {
  if (duty < DUTY_MIN) return DUTY_MIN;
  if (duty > DUTY_MAX) return DUTY_MAX;
  return duty;
}

static uint8_t dutyFromPower(uint8_t powerPercent) {
  if (powerPercent == 0) return 0;
  uint8_t inverted = 100 - powerPercent;
  return clampDuty(inverted);
}

void setupTimer1CTC() {
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS10);
  OCR1A  = T1_PERIOD_TICKS;
  TIMSK1 |= (1 << OCIE1A);
}

static inline FastPin makeFastPin(uint8_t pin) {
  FastPin fastPin;
  fastPin.out = portOutputRegister(digitalPinToPort(pin));
  fastPin.mask = digitalPinToBitMask(pin);
  return fastPin;
}

static inline void fastWrite(const FastPin &pin, bool high) {
  if (high) {
    *pin.out |= pin.mask;
  } else {
    *pin.out &= ~pin.mask;
  }
}

static inline void updatePwm(volatile bool &enabled,
                             volatile bool &pinHigh,
                             volatile uint16_t &ctr,
                             volatile uint16_t &onTicks,
                             volatile uint16_t &offTicks,
                             const FastPin &pin) {
  if (!enabled || !onTicks) return;

  uint16_t next = ++ctr;
  if (pinHigh) {
    if (next >= onTicks) {
      fastWrite(pin, LOW);
      pinHigh = false;
      ctr = 0;
    }
  } else {
    if (next >= offTicks) {
      fastWrite(pin, HIGH);
      pinHigh = true;
      ctr = 0;
    }
  }
}

void setPeltierCooling() {
  digitalWrite(PELTIER_EN_PIN, LOW);
  delay(50);
  digitalWrite(PELTIER_SWITCH_1_PIN, LOW);
  digitalWrite(PELTIER_SWITCH_2_PIN, LOW);
  delay(150);
  digitalWrite(PELTIER_SWITCH_1_PIN, HIGH);
}

void setPeltierHeating() {
  digitalWrite(PELTIER_EN_PIN, LOW);
  delay(50);
  digitalWrite(PELTIER_SWITCH_1_PIN, LOW);
  digitalWrite(PELTIER_SWITCH_2_PIN, LOW);
  delay(150);
  digitalWrite(PELTIER_SWITCH_2_PIN, HIGH);
}

void disablePeltier() {
  noInterrupts();
  peltierEnabled = false;
  peltierOnTicks = 0;
  peltierOffTicks = T1_PERIOD_TICKS;
  peltierDutyPercent = 0;
  interrupts();

  digitalWrite(PELTIER_PWM_PIN, LOW);
  digitalWrite(PELTIER_EN_PIN, LOW);
  digitalWrite(PELTIER_SWITCH_1_PIN, LOW);
  digitalWrite(PELTIER_SWITCH_2_PIN, LOW);
  peltierMode = PELTIER_MODE_UNKNOWN;
}

void enablePeltier(int8_t percent) {
  bool heating = (percent < 0);
  uint8_t duty = abs(percent);

  PeltierMode nextMode = heating ? PELTIER_MODE_HEATING : PELTIER_MODE_COOLING;
  if (peltierMode != nextMode) {
    if (heating) setPeltierHeating();
    else setPeltierCooling();
    peltierMode = nextMode;
  }

  noInterrupts();
  peltierOnTicks = dutyToTicks(duty);
  peltierOffTicks = T1_PERIOD_TICKS - peltierOnTicks;
  peltierEnabled = true;
  peltierCtr = 0;
  peltierDutyPercent = percent;
  interrupts();

  digitalWrite(PELTIER_EN_PIN, HIGH);
}

void disableHeater() {
  noInterrupts();
  heaterEnabled = false;
  heaterOnTicks = 0;
  heaterOffTicks = T1_PERIOD_TICKS;
  heaterDutyPercent = 0;
  interrupts();

  digitalWrite(HEATER_PWM_PIN, LOW);
  digitalWrite(HEATER_EN_PIN, LOW);
}

void enableHeater(uint8_t percent) {
  noInterrupts();
  heaterOnTicks = dutyToTicks(percent);
  heaterOffTicks = T1_PERIOD_TICKS - heaterOnTicks;
  heaterEnabled = true;
  heaterCtr = 0;
  heaterDutyPercent = percent;
  interrupts();

  digitalWrite(HEATER_EN_PIN, HIGH);
}

void disableMotor() {
  noInterrupts();
  motorEnabled = false;
  motorOnTicks = 0;
  motorOffTicks = T1_PERIOD_TICKS;
  interrupts();

  digitalWrite(MOTOR_PWM_PIN, LOW);
}

void enableMotor(uint8_t percent) {
  noInterrupts();
  motorOnTicks = dutyToTicks(percent);
  motorOffTicks = T1_PERIOD_TICKS - motorOnTicks;
  motorEnabled = true;
  motorCtr = 0;
  interrupts();
}

void countPulse() {
  rpmPulseCount++;
}

void updateRpm() {
  unsigned long now = millis();
  if (now - lastRpmMs < RPM_UPDATE_MS) return;

  noInterrupts();
  unsigned long pulses = rpmPulseCount;
  rpmPulseCount = 0;
  interrupts();

  unsigned long dt = now - lastRpmMs;
  if (dt > 0) {
    currentRpm = (pulses * 60000.0f) / (PULSES_PER_REV * dt);
  }
  lastRpmMs = now;
}

bool readRTD(Adafruit_MAX31865 &rtd, float &outTemp) {
  rtd.readRTD();
  float temperature = rtd.temperature(100, RREF);
  uint8_t fault = rtd.readFault();
  if (fault) {
    rtd.clearFault();
    return false;
  }
  outTemp = temperature;
  return true;
}

float controlTemperature() {
  if (externalPresent) return currentTempExternal;
  return currentTempInner;
}

void applyThermalControl() {
  float tempNow = controlTemperature();
  float err = targetTemp - tempNow;

  if (fabs(err) <= TEMP_HYSTERESIS) {
    disableHeater();
    disablePeltier();
    return;
  }

  if (err > 0) {
    uint8_t heaterPower = (uint8_t)min(100.0f, err * HEAT_K);
    uint8_t heaterDuty = dutyFromPower(heaterPower);
    if (heaterDuty == 0) {
      disableHeater();
    } else {
      enableHeater(heaterDuty);
    }

    if (err >= HEAT_ASSIST_THRESHOLD) {
      uint8_t assistPower = (uint8_t)min(100.0f, (err - HEAT_ASSIST_THRESHOLD) * PELTIER_HEAT_K);
      uint8_t assistDuty = dutyFromPower(assistPower);
      if (assistDuty == 0) {
        disablePeltier();
      } else {
        enablePeltier(-assistDuty);
      }
    } else {
      disablePeltier();
    }
  } else {
    uint8_t peltierPower = (uint8_t)min(100.0f, fabs(err) * COOL_K);
    uint8_t peltierDuty = dutyFromPower(peltierPower);
    disableHeater();
    if (peltierDuty == 0) {
      disablePeltier();
    } else {
      enablePeltier((int8_t)peltierDuty);
    }
  }
}

void applyMotorControl() {
  int rpmCommand = targetRpm;
  int rpmAbs = abs(rpmCommand);

  digitalWrite(MOTOR_SELECT_PIN, targetMotorSelect == 0 ? LOW : HIGH);
  digitalWrite(MOTOR_DIR_PIN, HIGH);

  if (rpmAbs <= 0) {
    motorDutyPercent = 0;
    disableMotor();
    return;
  }

  float error = (float)rpmAbs - currentRpm;
  float delta = error * MOTOR_KP;
  int nextDuty = (int)motorDutyPercent + (int)delta;

  if (nextDuty < 0) nextDuty = 0;
  if (nextDuty > 100) nextDuty = 100;

  motorDutyPercent = (uint8_t)nextDuty;
  enableMotor(motorDutyPercent);
}

int calculateChecksum(const char* message) {
  int checksum = 0;
  for (int i = 0; message[i] != '\0'; ++i) {
    checksum += message[i];
  }
  return checksum;
}

void sendStatus() {
  char innerStr[10];
  char extStr[10];
  char ampStr[10];
  char targetTempStr[10];
  char message[128];
  char output[140];

  dtostrf(currentTempInner, 4, 1, innerStr);
  dtostrf(currentTempExternal, 4, 1, extStr);
  dtostrf((float)motorDutyPercent, 3, 0, ampStr);
  dtostrf(targetTemp, 4, 1, targetTempStr);

  snprintf(message, sizeof(message), "%s#%s#%s#%d#%s#%s#%d",
           CELL_ID,
           innerStr,
           extStr,
           (int)currentRpm,
           ampStr,
           targetTempStr,
           targetRpm);

  int checksum = calculateChecksum(message);
  snprintf(output, sizeof(output), "%s*%d", message, checksum);
  Serial.println(output);
}

void sendDebug() {
  char output[64];
  int8_t peltierDuty = peltierDutyPercent;
  uint8_t heaterDuty = heaterDutyPercent;
  snprintf(output, sizeof(output), "DEBUG#HEATER:%u#PELTIER:%d", heaterDuty, peltierDuty);
  Serial.println(output);
}

bool handleUpdateCommand(const char* line) {
  if (!line || line[0] == '\0') return false;
  if (line[0] != '>' || line[strlen(line) - 1] != '<') return false;

  char payload[128];
  strncpy(payload, line + 1, sizeof(payload));
  payload[sizeof(payload) - 1] = '\0';
  size_t payloadLen = strlen(payload);
  if (payloadLen == 0) return false;
  payload[payloadLen - 1] = '\0';

  char* token = strtok(payload, "#");
  if (!token) return false;
  int positionIdx = atoi(token);

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

  int checksumCalc = positionIdx + (int)tTemp + tRpm + tMotorSelect;
  if (checksumRec != checksumCalc) return false;

  targetTemp = tTemp;
  targetRpm = tRpm;
  targetMotorSelect = (uint8_t)tMotorSelect;
  return true;
}

bool handlePollCommand(const char* line) {
  if (!line) return false;
  while (*line && isspace(static_cast<unsigned char>(*line))) {
    line++;
  }
  if (*line == '\0') return false;
  if (strcmp(line, "GO") == 0) {
    sendStatus();
    return true;
  }
  if (strcmp(line, "DEBUG") == 0) {
    sendDebug();
    return true;
  }
  return false;
}

void setup() {
  Serial.begin(BAUD);

  pinMode(USB_EN_PIN, OUTPUT);
  digitalWrite(USB_EN_PIN, HIGH);

  pinMode(PELTIER_PWM_PIN, OUTPUT);
  pinMode(PELTIER_SWITCH_1_PIN, OUTPUT);
  pinMode(PELTIER_SWITCH_2_PIN, OUTPUT);
  pinMode(PELTIER_EN_PIN, OUTPUT);

  pinMode(HEATER_PWM_PIN, OUTPUT);
  pinMode(HEATER_EN_PIN, OUTPUT);

  pinMode(MOTOR_SELECT_PIN, OUTPUT);
  pinMode(MOTOR_DIR_PIN, OUTPUT);
  pinMode(MOTOR_PWM_PIN, OUTPUT);
  pinMode(MOTOR_FB_PIN, INPUT_PULLUP);

  peltierPwmPin = makeFastPin(PELTIER_PWM_PIN);
  heaterPwmPin = makeFastPin(HEATER_PWM_PIN);
  motorPwmPin = makeFastPin(MOTOR_PWM_PIN);

  disablePeltier();
  disableHeater();
  disableMotor();

  rtdInner.begin(MAX31865_2WIRE);
  rtdExternal.begin(MAX31865_3WIRE);

  attachInterrupt(digitalPinToInterrupt(MOTOR_FB_PIN), countPulse, FALLING);

  setupTimer1CTC();

  lastRpmMs = millis();
}

void loop() {
  float tempInner;
  if (readRTD(rtdInner, tempInner)) {
    currentTempInner = tempInner;
  }

  float tempExt;
  if (readRTD(rtdExternal, tempExt)) {
    currentTempExternal = tempExt;
    externalPresent = true;
  } else {
    externalPresent = false;
    currentTempExternal = currentTempInner;
  }

  updateRpm();
  applyMotorControl();
  applyThermalControl();

  if (Serial.available() > 0) {
    char line[128];
    size_t len = Serial.readBytesUntil('\n', line, sizeof(line) - 1);
    line[len] = '\0';
    if (!handleUpdateCommand(line)) {
      handlePollCommand(line);
    }
  }
}

ISR(TIMER1_COMPA_vect) {
  updatePwm(peltierEnabled, peltierPinHigh, peltierCtr, peltierOnTicks, peltierOffTicks, peltierPwmPin);
  updatePwm(heaterEnabled, heaterPinHigh, heaterCtr, heaterOnTicks, heaterOffTicks, heaterPwmPin);
  updatePwm(motorEnabled, motorPinHigh, motorCtr, motorOnTicks, motorOffTicks, motorPwmPin);
}
