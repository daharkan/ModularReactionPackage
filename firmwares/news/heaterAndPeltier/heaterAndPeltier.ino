#include <Arduino.h>

/* ===================== PINLER ===================== */
// ---- PELTIER ----
#define PELTIER_PWM_PIN       4
#define PELTIER_SWITCH_1_PIN  14
#define PELTIER_SWITCH_2_PIN  19
#define PELTIER_EN_PIN        25

// ---- HEATER ----
#define HEATER_EN_PIN   10
#define HEATER_PWM_PIN  A4

#define USB_EN_PIN 24

// ---- TIMER ----
#define T1_PERIOD_TICKS 266   // ~30 kHz @16MHz, prescaler yok

/* ===================== PELTIER PWM STATE ===================== */
volatile bool     peltierEnabled = false;
volatile bool     peltierPinHigh = false;
volatile uint16_t peltierOnTicks  = 0;
volatile uint16_t peltierOffTicks = T1_PERIOD_TICKS;
volatile uint16_t peltierCtr = 0;

/* ===================== HEATER PWM STATE ===================== */
volatile bool     heaterEnabled = false;
volatile bool     heaterPinHigh = false;
volatile uint16_t heaterOnTicks  = 0;
volatile uint16_t heaterOffTicks = T1_PERIOD_TICKS;
volatile uint16_t heaterCtr = 0;

/* ===================== ORTAK ===================== */
static inline uint16_t dutyToTicks(uint8_t percent)
{
  if (percent > 100) percent = 100;
  return (uint32_t)percent * T1_PERIOD_TICKS / 100u;
}

/* ===================== TIMER1 ===================== */
void setupTimer1CTC()
{
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS10);
  OCR1A  = T1_PERIOD_TICKS;
  TIMSK1 |= (1 << OCIE1A);
}

/* ===================== PELTIER YÖN ===================== */
void setPeltierCooling()
{
  digitalWrite(PELTIER_EN_PIN, LOW);
  delay(100);
  digitalWrite(PELTIER_SWITCH_1_PIN, LOW);
  digitalWrite(PELTIER_SWITCH_2_PIN, LOW);
  delay(300);
  digitalWrite(PELTIER_SWITCH_1_PIN, HIGH);
}

void setPeltierHeating()
{
  digitalWrite(PELTIER_EN_PIN, LOW);
  delay(100);
  digitalWrite(PELTIER_SWITCH_1_PIN, LOW);
  digitalWrite(PELTIER_SWITCH_2_PIN, LOW);
  delay(300);
  digitalWrite(PELTIER_SWITCH_2_PIN, HIGH);
}

/* ===================== PELTIER ===================== */
void disablePeltier()
{
  noInterrupts();
  peltierEnabled = false;
  peltierOnTicks = 0;
  peltierOffTicks = T1_PERIOD_TICKS;
  interrupts();

  digitalWrite(PELTIER_PWM_PIN, LOW);
  digitalWrite(PELTIER_EN_PIN, LOW);
  digitalWrite(PELTIER_SWITCH_1_PIN, LOW);
  digitalWrite(PELTIER_SWITCH_2_PIN, LOW);

  Serial.println(F("Peltier OFF"));
}

void enablePeltier(int8_t percent)
{
  bool heating = (percent < 0);
  uint8_t duty = abs(percent);

  if (heating) setPeltierHeating();
  else         setPeltierCooling();

  noInterrupts();
  peltierOnTicks  = dutyToTicks(duty);
  peltierOffTicks = T1_PERIOD_TICKS - peltierOnTicks;
  peltierEnabled  = true;
  peltierCtr = 0;
  interrupts();

  digitalWrite(PELTIER_EN_PIN, HIGH);

  Serial.print(F("Peltier "));
  Serial.print(heating ? F("HEAT ") : F("COOL "));
  Serial.print(duty);
  Serial.println(F("%"));
}

/* ===================== HEATER ===================== */
void disableHeater()
{
  noInterrupts();
  heaterEnabled = false;
  heaterOnTicks = 0;
  heaterOffTicks = T1_PERIOD_TICKS;
  interrupts();

  digitalWrite(HEATER_PWM_PIN, LOW);
  digitalWrite(HEATER_EN_PIN, LOW);

  Serial.println(F("Heater OFF"));
}

void enableHeater(uint8_t percent)
{
  noInterrupts();
  heaterOnTicks  = dutyToTicks(percent);
  heaterOffTicks = T1_PERIOD_TICKS - heaterOnTicks;
  heaterEnabled  = true;
  heaterCtr = 0;
  interrupts();

  digitalWrite(HEATER_EN_PIN, HIGH);

  Serial.print(F("Heater "));
  Serial.print(percent);
  Serial.println(F("%"));
}

/* ===================== SETUP ===================== */
void setup()
{
  Serial.begin(9600);

  pinMode(PELTIER_PWM_PIN, OUTPUT);
  pinMode(PELTIER_SWITCH_1_PIN, OUTPUT);
  pinMode(PELTIER_SWITCH_2_PIN, OUTPUT);
  pinMode(PELTIER_EN_PIN, OUTPUT);

  pinMode(HEATER_PWM_PIN, OUTPUT);
  pinMode(HEATER_EN_PIN, OUTPUT);

  pinMode(USB_EN_PIN, OUTPUT);
  digitalWrite(USB_EN_PIN, HIGH);

  disablePeltier();
  disableHeater();

  setupTimer1CTC();

  Serial.println(F("READdddd--------ddddY"));
  Serial.println(F("Commands:"));
  Serial.println(F("P 30   -> Peltier COOL %30"));
  Serial.println(F("P -40  -> Peltier HEAT %40"));
  Serial.println(F("H 25   -> Heater %25"));
}

/* ===================== LOOP ===================== */
void loop()
{
  if (!Serial.available()) return;

  char dev = Serial.read();
  long val = Serial.parseInt();

  if (dev == 'P' || dev == 'p') {
    if (val == 0) disablePeltier();
    else enablePeltier(val);
  }

  if (dev == 'H' || dev == 'h') {
    if (val == 0) disableHeater();
    else enableHeater(val);
  }
}

/* ===================== ISR ===================== */
ISR(TIMER1_COMPA_vect)
{
  /* ---- PELTIER PWM ---- */
  if (peltierEnabled && peltierOnTicks) {
    if (peltierPinHigh) {
      if (++peltierCtr >= peltierOnTicks) {
        digitalWrite(PELTIER_PWM_PIN, LOW);
        peltierPinHigh = false;
        peltierCtr = 0;
      }
    } else {
      if (++peltierCtr >= peltierOffTicks) {
        digitalWrite(PELTIER_PWM_PIN, HIGH);
        peltierPinHigh = true;
        peltierCtr = 0;
      }
    }
  }

  /* ---- HEATER PWM ---- */
  if (heaterEnabled && heaterOnTicks) {
    if (heaterPinHigh) {
      if (++heaterCtr >= heaterOnTicks) {
        digitalWrite(HEATER_PWM_PIN, LOW);
        heaterPinHigh = false;
        heaterCtr = 0;
      }
    } else {
      if (++heaterCtr >= heaterOffTicks) {
        digitalWrite(HEATER_PWM_PIN, HIGH);
        heaterPinHigh = true;
        heaterCtr = 0;
      }
    }
  }
}
