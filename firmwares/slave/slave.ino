#include <Arduino.h>
#include "FastPin.h"
#include <math.h>
#include <SPI.h>
#include <Adafruit_MAX31865.h>
#include <ctype.h>

// ===================== ID / POSITION =====================
#define CELL_ID "s25_555"

// ===================== RTD PINS =====================
#define RREF 430.0
#define RTD_CS_INNER 2
#define RTD_CS_EXTERNAL 5

// ===================== PELTIER PINS =====================
#define PELTIER_PWM_PIN       4
#define PELTIER_SWITCH_1_PIN  14
#define PELTIER_SWITCH_2_PIN  19
#define PELTIER_EN_PIN        25

// ===================== HEATER PINS =====================
#define HEATER_EN_PIN   10
#define HEATER_PWM_PIN  A4

// ===================== MOTOR PINS =====================
#define MOTOR_SELECT_PIN 7
#define MOTOR_DIR_PIN    26
#define MOTOR_PWM_PIN    23
#define MOTOR_FB_PIN      3
#define PULSES_PER_REV    21
#define USB_EN_PIN 24

// ===================== TIMER & PWM CONSTANTS =====================
#define T1_PERIOD_TICKS 532 
constexpr uint32_t BAUD = 9600;
constexpr uint16_t RPM_UPDATE_MS = 2000;
constexpr float MOTOR_KP = 0.08f; 
constexpr float RPM_SCALE_FACTOR = 9.0f; 
constexpr uint8_t DUTY_MIN = 15; 
constexpr uint8_t DUTY_MAX = 55; 
constexpr uint8_t PWM_DUTY_STEPS = 50;

// ===================== THERMAL TUNING =====================
constexpr float TEMP_HYSTERESIS = 0.35f;           
constexpr float KNOB_HEAT_SPEED = 1.60f;           
constexpr float KNOB_COOL_SPEED = 1.00f;           
constexpr float KNOB_OVERSHOOT_GUARD = 0.60f;      
constexpr float KNOB_PREDICTIVE = 0.10f;           
constexpr unsigned long MODE_HOLD_MS = 5000;       
constexpr float REVERSAL_GAP_BYPASS_EPS = 6.0f;    

// ===================== BUFFER SIZES =====================
constexpr size_t SERIAL_LINE_SIZE = 192;
constexpr size_t UPDATE_PAYLOAD_SIZE = 192;
constexpr size_t STATUS_MSG_SIZE = 160;
constexpr size_t STATUS_OUT_SIZE = 192;

// ===================== DERIVED CONSTANTS =====================
constexpr float clampf(float value, float low, float high) {
    return (value < low) ? low : ((value > high) ? high : value);
}
constexpr float HEAT_K = 8.0f * clampf(KNOB_HEAT_SPEED, 0.5f, 2.0f);   
constexpr float COOL_K = 16.0f * clampf(KNOB_COOL_SPEED, 0.5f, 2.0f);  
constexpr float HEAT_ASSIST_THRESHOLD = 6.0f / clampf(KNOB_HEAT_SPEED, 0.7f, 1.8f);
constexpr float PELTIER_HEAT_K = 6.0f * clampf(KNOB_HEAT_SPEED, 0.6f, 1.8f);
constexpr float EMERGENCY_SWITCH_ERR = 4.0f * clampf(KNOB_OVERSHOOT_GUARD, 0.3f, 1.6f);
constexpr float EXT_GAP_PREDICT_K = 0.40f * clampf(KNOB_PREDICTIVE, 0.1f, 1.8f);
constexpr float EXT_RATE_PREDICT_SEC = 3.0f * clampf(KNOB_PREDICTIVE, 0.1f, 1.8f);
constexpr float EXT_PREDICT_OFFSET_CLAMP = 10.0f * clampf(KNOB_PREDICTIVE, 0.1f, 1.6f);

constexpr float FUTURE_TREND_EPS = 0.15f;  
constexpr float LEAD_EPS = 0.3f;           
constexpr float PLATEAU_EPS = 1.5f;        
constexpr float PLATEAU_MIN_POWER = 35.0f; 
constexpr float FUTURE_PREP_LEAD_EPS = 0.35f;
constexpr float FUTURE_PREP_POWER = 25.0f;
constexpr float FUTURE_FULL_POWER_ERR_MIN = 0.4f;
constexpr float HEAT_HOLD_ON_LAG = 0.8f;   // hysteresis bandinda isi tutmak icin raw external lag on esigi
constexpr float HEAT_HOLD_OFF_LAG = 0.2f;  // lag bu degerin altina inerse heat hold birakilir

// ===================== GLOBAL OBJECTS =====================
Adafruit_MAX31865 rtdInner = Adafruit_MAX31865(RTD_CS_INNER);
Adafruit_MAX31865 rtdExternal = Adafruit_MAX31865(RTD_CS_EXTERNAL);
FastPin peltierPwmPin;
FastPin heaterPwmPin;
FastPin motorPwmPin;

// ===================== VOLATILE PWM & STATE =====================
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

float currentTempInner = 0.0f;
float currentTempExternal = 0.0f;
bool externalPresent = false;
float targetTemp = 25.0f;
float targetTempFuture = 25.0f;
int targetRpm = 0;
uint8_t targetMotorSelect = 0;
volatile unsigned long rpmPulseCount = 0;
unsigned long lastRpmMs = 0;
float currentRpm = 0.0f;
uint8_t motorDutyPercent = 0;
bool controlEnabled = false;

enum ThermalMode : uint8_t {
    MODE_IDLE = 0,
    MODE_HEAT,
    MODE_COOL
};
ThermalMode activeThermalMode = MODE_IDLE;
ThermalMode lastDrivenThermalMode = MODE_IDLE;
unsigned long lastDrivenModeChangedAtMs = 0;

float errorIntegral = 0.0f;
unsigned long lastIntegralUpdateMs = 0;
bool heatHoldLatched = false;
float extRateFilt = 0.0f;
float lastExtTemp = 0.0f;
unsigned long lastExtRateMs = 0;
constexpr float EXT_RATE_ALPHA = 0.70f;

// ===================== UTILS =====================
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
    return clampDuty(100 - powerPercent);
}

void setupTimer1CTC() {
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS10);
    OCR1A  = T1_PERIOD_TICKS;
    TIMSK1 |= (1 << OCIE1A);
}

static inline FastPin makeFastPin(uint8_t pin) {
    FastPin fp;
    fp.out = portOutputRegister(digitalPinToPort(pin));
    fp.mask = digitalPinToBitMask(pin);
    return fp;
}

static inline void fastWrite(const FastPin &pin, bool high) {
    if (high) *pin.out |= pin.mask;
    else      *pin.out &= ~pin.mask;
}

static inline void updatePwm(volatile bool &enabled, volatile bool &pinHigh, volatile uint16_t &ctr, volatile uint16_t &onTicks, volatile uint16_t &offTicks, const FastPin &pin) {
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

// ===================== ACTUATORS (TEK SATIRLAR DÜZELTİLDİ) =====================
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

// ===================== SENSORS =====================
void countPulse() {
    rpmPulseCount++;
}

void updateRpm() {
    unsigned long now = millis();
    if (now - lastRpmMs < RPM_UPDATE_MS) {
        return;
    }
    noInterrupts();
    unsigned long pulses = rpmPulseCount;
    rpmPulseCount = 0;
    interrupts();
    unsigned long dt = now - lastRpmMs;
    if (dt > 0) {
        currentRpm = ((pulses * 60000.0f) / (PULSES_PER_REV * dt)) / RPM_SCALE_FACTOR;
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
    if (externalPresent) {
        float gapTerm = (currentTempInner - currentTempExternal) * EXT_GAP_PREDICT_K;
        float rateTerm = extRateFilt * EXT_RATE_PREDICT_SEC;
        float predictedOffset = gapTerm + rateTerm;
        predictedOffset = clampf(predictedOffset, -EXT_PREDICT_OFFSET_CLAMP, EXT_PREDICT_OFFSET_CLAMP);
        return currentTempExternal + predictedOffset;
    }
    return currentTempInner;
}

void updateExternalRate() {
    unsigned long now = millis();
    if (!externalPresent) {
        extRateFilt = 0.0f;
        lastExtRateMs = now;
        lastExtTemp = currentTempExternal;
        return;
    }
    if (lastExtRateMs == 0) {
        lastExtRateMs = now;
        lastExtTemp = currentTempExternal;
        extRateFilt = 0.0f;
        return;
    }
    unsigned long dtMs = now - lastExtRateMs;
    if (dtMs < 1000) {
        return;
    } 
    float dt = dtMs / 1000.0f;
    float rate = (currentTempExternal - lastExtTemp) / dt; 
    extRateFilt = (1.0f - EXT_RATE_ALPHA) * extRateFilt + EXT_RATE_ALPHA * rate;
    lastExtRateMs = now;
    lastExtTemp = currentTempExternal;
}

// ===================== CORE THERMAL CONTROL =====================
void applyThermalControl() {
    float controlTarget = targetTemp;
    float tempNow = controlTemperature(); 
    float err = controlTarget - tempNow;
    float errAbs = fabs(err);
    float futureDelta = targetTempFuture - targetTemp;

    unsigned long now = millis();
    if (now - lastIntegralUpdateMs >= 100) {
        float dt = (lastIntegralUpdateMs == 0) ? 0.1f : ((now - lastIntegralUpdateMs) / 1000.0f);
        lastIntegralUpdateMs = now;

        // Integral sadece isitmaye yardim etsin; mode secimini zorlamasin.
        if (err > 0.0f && errAbs < 3.0f && activeThermalMode != MODE_COOL) {
            errorIntegral += err * 0.8f * dt;
        } else {
            errorIntegral *= 0.95f;
        }

        if (err < -0.2f || activeThermalMode == MODE_COOL) {
            errorIntegral = 0.0f;
        }
        errorIntegral = clampf(errorIntegral, 0.0f, 30.0f);
    }

    // Histerezis bandinda heat-idle chatter'ini azaltmak icin latch.
    float heatLagRef = controlTarget - (externalPresent ? currentTempExternal : tempNow);
    if (futureDelta < -FUTURE_TREND_EPS || heatLagRef < HEAT_HOLD_OFF_LAG) {
        heatHoldLatched = false;
    } else if (heatLagRef > HEAT_HOLD_ON_LAG) {
        heatHoldLatched = true;
    }

    ThermalMode desiredMode = MODE_IDLE;
    bool prepActive = false;
    float prepPowerF = 0.0f;

    if (errAbs > TEMP_HYSTERESIS) {
        desiredMode = (err > 0.0f) ? MODE_HEAT : MODE_COOL;
    } else {
        if (heatHoldLatched && err > -TEMP_HYSTERESIS) {
            desiredMode = MODE_HEAT;
        } else {
            float trendRef = externalPresent ? currentTempExternal : tempNow;
            bool futureUp = (futureDelta > FUTURE_TREND_EPS);
            bool futureDown = (futureDelta < -FUTURE_TREND_EPS);
            if (futureUp && (trendRef - controlTarget) < -FUTURE_PREP_LEAD_EPS) {
                desiredMode = MODE_HEAT;
                prepActive = true;
                prepPowerF = FUTURE_PREP_POWER;
            } else if (futureDown && (trendRef - controlTarget) > FUTURE_PREP_LEAD_EPS) {
                desiredMode = MODE_COOL;
                prepActive = true;
                prepPowerF = FUTURE_PREP_POWER;
            }
        }
    }

    if (desiredMode != MODE_IDLE && lastDrivenThermalMode != MODE_IDLE && desiredMode != lastDrivenThermalMode) {
        unsigned long elapsed = millis() - lastDrivenModeChangedAtMs;
        bool largeGap = externalPresent && (fabs(currentTempInner - currentTempExternal) > REVERSAL_GAP_BYPASS_EPS);
        if (elapsed < MODE_HOLD_MS && errAbs < EMERGENCY_SWITCH_ERR && !largeGap) {
            desiredMode = MODE_IDLE;
        }
    }

    if (desiredMode == MODE_IDLE) {
        disableHeater();
        disablePeltier();
    } 
    else if (desiredMode == MODE_HEAT) {
        float hPower = prepActive ? prepPowerF : (err * HEAT_K + errorIntegral);
        if (externalPresent && !prepActive) {
            float lag = controlTarget - currentTempExternal;
            if (targetTempFuture > targetTemp + FUTURE_TREND_EPS) {
                if (lag > 0.1f) hPower += lag * (HEAT_K * 1.5f);
                if (lag > LEAD_EPS && lag > FUTURE_FULL_POWER_ERR_MIN) {
                    hPower = 100.0f;
                    errorIntegral = 0.0f;
                }
            }
            if (!(targetTempFuture > targetTemp + FUTURE_TREND_EPS) && (max(lag, (controlTarget - tempNow)) > PLATEAU_EPS)) {
                if (hPower < PLATEAU_MIN_POWER) hPower = PLATEAU_MIN_POWER;
            }
        }
        hPower = clampf(hPower, 0.0f, 100.0f);
        uint8_t hDuty = dutyFromPower((uint8_t)hPower);
        if (hDuty == 0) {
            disableHeater(); 
        } else {
            enableHeater(hDuty);
        }

        if (!prepActive && (err >= HEAT_ASSIST_THRESHOLD || hPower > 80.0f)) {
            float assist = (err >= HEAT_ASSIST_THRESHOLD) ? (err - HEAT_ASSIST_THRESHOLD) * PELTIER_HEAT_K : (hPower * 0.8f);
            uint8_t assistDuty = dutyFromPower((uint8_t)clampf(assist, 0.0f, 100.0f));
            if (assistDuty == 0) {
                disablePeltier();
            } else {
                enablePeltier(-(int8_t)assistDuty);
            }
        } else {
            disablePeltier();
        }
    } 
    else { // MODE_COOL
        float pPower = prepActive ? prepPowerF : (errAbs * COOL_K);
        if (externalPresent && !prepActive) {
            float hotLead = currentTempExternal - controlTarget;
            if (targetTempFuture < targetTemp - FUTURE_TREND_EPS && hotLead > LEAD_EPS && hotLead > FUTURE_FULL_POWER_ERR_MIN) {
                pPower = 100.0f;
            } else if (!(targetTempFuture < targetTemp - FUTURE_TREND_EPS) && max(hotLead, (tempNow - controlTarget)) > PLATEAU_EPS) {
                if (pPower < PLATEAU_MIN_POWER) pPower = PLATEAU_MIN_POWER;
            }
        }
        pPower = clampf(pPower, 0.0f, 100.0f);
        disableHeater(); 
        uint8_t pDuty = dutyFromPower((uint8_t)pPower);
        if (pDuty == 0) {
            disablePeltier();
        } else {
            enablePeltier((int8_t)pDuty);
        }
    }

    activeThermalMode = desiredMode;
    if (desiredMode != MODE_IDLE && desiredMode != lastDrivenThermalMode) {
        lastDrivenThermalMode = desiredMode;
        lastDrivenModeChangedAtMs = millis();
    }
}

void applyMotorControl() {
    int rpmAbs = abs(targetRpm);
    if (targetMotorSelect == 0) {
        digitalWrite(MOTOR_SELECT_PIN, LOW);
    } else {
        digitalWrite(MOTOR_SELECT_PIN, HIGH);
    }
    digitalWrite(MOTOR_DIR_PIN, HIGH);
    if (rpmAbs <= 0) {
        motorDutyPercent = 0;
        disableMotor();
        return;
    }
    float delta = ((float)rpmAbs - currentRpm) * MOTOR_KP;
    int nextDuty = (int)motorDutyPercent + (int)delta;
    motorDutyPercent = (uint8_t)clampf(nextDuty, 0, 100);
    enableMotor(motorDutyPercent);
}

// ===================== COMMUNICATION =====================
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
    char message[STATUS_MSG_SIZE];
    char output[STATUS_OUT_SIZE];

    dtostrf(currentTempInner, 4, 1, innerStr);
    dtostrf(currentTempExternal, 4, 1, extStr);
    dtostrf((float)motorDutyPercent, 3, 0, ampStr);
    dtostrf(targetTemp, 4, 1, targetTempStr);

    snprintf(message, sizeof(message), "%s#%s#%s#%d#%s#%s#%d", CELL_ID, innerStr, extStr, (int)currentRpm, ampStr, targetTempStr, targetRpm);
    snprintf(output, sizeof(output), "%s*%d", message, calculateChecksum(message));
    Serial.println(output);
}

void sendDebug() {
    char output[80];
    snprintf(output, sizeof(output), "DEBUG#HEATER:%u#PELTIER:%d#I:%.1f", heaterDutyPercent, peltierDutyPercent, errorIntegral);
    Serial.println(output);
}

bool handleUpdateCommand(const char* line) {
    if (!line || line[0] != '>' || line[strlen(line)-1] != '<') return false;
    char payload[UPDATE_PAYLOAD_SIZE];
    strncpy(payload, line + 1, sizeof(payload));
    payload[strlen(payload)-1] = '\0';

    char* tok1 = strtok(payload, "#");
    char* tok2 = strtok(nullptr, "#");
    char* tok3 = strtok(nullptr, "#");
    char* tok4 = strtok(nullptr, "#");
    char* tok5 = strtok(nullptr, "#");
    char* tok6 = strtok(nullptr, "#");
    
    if (!tok6) return false;

    int pos = atoi(tok1);
    float tT = atof(tok2);
    int tR = atoi(tok3);
    int tM = atoi(tok4);
    float tF = atof(tok5);
    int cR = atoi(tok6);

    if (cR != (pos + (int)tT + tR + tM + (int)tF)) return false;

    targetTemp = tT;
    targetRpm = tR;
    targetMotorSelect = (uint8_t)tM;
    targetTempFuture = tF;
    controlEnabled = true;
    return true;
}

bool handlePollCommand(const char* line) {
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

// ===================== ARDUINO CORE =====================
void setup() {
    pinMode(PELTIER_PWM_PIN, OUTPUT);
    pinMode(PELTIER_SWITCH_1_PIN, OUTPUT);
    pinMode(PELTIER_SWITCH_2_PIN, OUTPUT);
    pinMode(PELTIER_EN_PIN, OUTPUT);
    pinMode(HEATER_PWM_PIN, OUTPUT);
    pinMode(HEATER_EN_PIN, OUTPUT);
    pinMode(MOTOR_SELECT_PIN, OUTPUT);
    pinMode(MOTOR_DIR_PIN, OUTPUT);
    pinMode(MOTOR_PWM_PIN, OUTPUT);
    pinMode(USB_EN_PIN, OUTPUT);
    digitalWrite(USB_EN_PIN, HIGH);
    pinMode(MOTOR_FB_PIN, INPUT_PULLUP);

    Serial.begin(BAUD);

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
    targetTempFuture = targetTemp;
}

void loop() {
    float tI, tE;
    if (readRTD(rtdInner, tI)) {
        currentTempInner = tI;
    }
    if (readRTD(rtdExternal, tE)) {
        currentTempExternal = tE;
        externalPresent = true;
    } else {
        externalPresent = false;
        currentTempExternal = currentTempInner;
    }

    updateExternalRate();
    updateRpm();

    if (controlEnabled) {
        applyMotorControl();
        applyThermalControl();
    } else {
        disableMotor();
        disableHeater();
        disablePeltier();
        errorIntegral = 0.0f;
        heatHoldLatched = false;
        activeThermalMode = MODE_IDLE;
    }

    if (Serial.available() > 0) {
        char line[SERIAL_LINE_SIZE];
        size_t len = Serial.readBytesUntil('\n', line, sizeof(line)-1);
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
