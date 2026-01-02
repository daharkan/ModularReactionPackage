#define MOTOR_SELECT_PIN 7   // PD7 - LOW: aktif
#define MOTOR_DIR_PIN    26  // PE3 - yön
#define MOTOR_PWM_PIN    23  // PE0 - MCP1416 input (donanım PWM yok)
#define MOTOR_FB_PIN      3  // PD3 - feedback
#define PULSES_PER_REV    2
#define T1_PERIOD_TICKS  266 // ~30kHz @ CTC, no prescale

volatile unsigned long pulseCount = 0;
unsigned long lastMillis = 0;
unsigned long lastMillis2 = 0;
uint8_t pwm = 0;

double currentRpm = 0;

// --- yazılım PWM durumu ---
volatile uint16_t onTicks = 0, offTicks = T1_PERIOD_TICKS;
volatile uint8_t  dutyEnabled = 0;

void countPulse() { pulseCount++; }

// PWM duty (0..255) -> on/off ticklerine çevir
inline void updatePWMTicks(uint8_t pwm255) {
  uint16_t on = (uint32_t)pwm255 * T1_PERIOD_TICKS / 255u;
  onTicks  = on;
  offTicks = T1_PERIOD_TICKS - on;
  dutyEnabled = (on > 0);
}

// Timer1 CTC ~30kHz kur
void setupTimer1CTC() {
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS10); // CTC, no prescale
  OCR1A  = T1_PERIOD_TICKS;
  TIMSK1 |= (1 << OCIE1A);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Motor PWM test (MCP1416).");

  pinMode(MOTOR_SELECT_PIN, OUTPUT);
  pinMode(MOTOR_DIR_PIN, OUTPUT);
  pinMode(MOTOR_PWM_PIN, OUTPUT);
  pinMode(MOTOR_FB_PIN, INPUT_PULLUP);

  // Doğru seçim ve yön
  digitalWrite(MOTOR_SELECT_PIN, HIGH);   // *** LOW = aktif ***
  digitalWrite(MOTOR_DIR_PIN, HIGH);     // gerekirse LOW deneyebilirsin
  digitalWrite(MOTOR_PWM_PIN, LOW);

  attachInterrupt(digitalPinToInterrupt(MOTOR_FB_PIN), countPulse, FALLING);

  setupTimer1CTC();
  updatePWMTicks(255);

  lastMillis  = millis();
  lastMillis2 = millis();
}

void loop() {
  // 5 sn'de bir PWM arttır
  if (millis() - lastMillis2 >= 5000) {
    pwm = (uint8_t)(pwm + 5);
    // taşma olursa 0'a sar
    // (uint8_t) olduğu için 255->260 geçişi zaten 5'e düşer
    updatePWMTicks(pwm);
    lastMillis2 = millis();

    Serial.print("PWM=");
    Serial.println(pwm);
  }

  // 1 sn'de bir RPM ölç
  if (millis() - lastMillis >= 1000) {
    noInterrupts();
    unsigned long pulses = pulseCount;
    pulseCount = 0;
    interrupts();

    unsigned long dt = millis() - lastMillis;
    currentRpm = (pulses * 60000.0) / (PULSES_PER_REV * dt);
    lastMillis = millis();

    Serial.print("RPM: ");
    Serial.println((long)currentRpm);
  }
}

// Timer1 Compare Match ISR: MOTOR_PWM_PIN için yazılım PWM
ISR(TIMER1_COMPA_vect) {
  static bool pinHigh = false;
  static uint16_t cnt = 0;

  if (!dutyEnabled) {
    if (pinHigh) { digitalWrite(MOTOR_PWM_PIN, LOW); pinHigh = false; }
    cnt = 0;
    return;
  }

  if (pinHigh) {
    if (cnt >= onTicks) {
      digitalWrite(MOTOR_PWM_PIN, LOW);
      pinHigh = false;
      cnt = 0;
    }
  } else {
    if (cnt >= offTicks) {
      digitalWrite(MOTOR_PWM_PIN, HIGH);
      pinHigh = true;
      cnt = 0;
    }
  }
  cnt++;
}
