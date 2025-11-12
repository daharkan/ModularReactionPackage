	#include <Adafruit_MAX31865.h>
	#include <SPI.h>
	#include <PID_v1.h>

	// RTD sensor setup
	#define RREF 430.0  // Reference resistor value
	#define MAX31865_CS1  2  // Chip select for RTD1 (2-Wire)
	#define MAX31865_CS2  5  // Chip select for RTD2 (4-Wire)

	// Peltier and heater setup
	#define PELTIER_PWM_PIN       4   // PD4 - hw 2
	#define PELTIER_SWITCH_1_PIN  10  // PB2 - hw 14   
	#define PELTIER_SWITCH_2_PIN  A5  // PC5 - hw 28 
	#define PELTIER_EN_PIN        25  // PE2 - hw 19
	#define HEATER_PWM_PIN        8   // PB0 - hw 12
	#define MOTOR_DIR_PIN         26  // PE3 - hw 22
	#define MOTOR_PWM_PIN         23  // PE0 - hw 3
	#define MOTOR_FB_PIN          3   // PD3- hw 1

	// Serial print constants
	#define BUSBOARD_PREFIX "bb00#s25_555"
	#define MESSAGE_LENGTH 100
	#define RPM_UPDATE_MSECS 1500
	// RTD sensor instance creation
	Adafruit_MAX31865 rtd1 = Adafruit_MAX31865(MAX31865_CS1);
	Adafruit_MAX31865 rtd2 = Adafruit_MAX31865(MAX31865_CS2);

	// PID variables
	double m_currentTempExternal, m_currentTempInner, m_targetTemperature, m_outputHeater, m_motorDutyCycle;
	double m_currentRpm = 0;
	double m_targetRpm = 0;
	unsigned long m_rpmTimingStartTime = 0;
	unsigned long m_rpmTimingEndTime = 0;


	double m_kp = 5, m_ki = 2.0, m_kd = 1;  // Initial PID parameters
	PID m_pidHeater(&m_currentTempExternal, &m_outputHeater, &m_targetTemperature, m_kp, m_ki, m_kd, DIRECT);

	//double m_kpMotor = 2, m_kiMotor = 5.0, m_kdMotor = 1;  // Initial PID parameters
	//PID m_pidMotor(&m_currentRpm, &m_motorDutyCycle, &m_targetRpm, m_kpMotor, m_kiMotor, m_kdMotor, DIRECT);

	// PWM variables
	unsigned long m_rpmCount = 0;
	volatile uint16_t m_onTimeHeater, m_offTimeHeater;
	volatile uint16_t m_onTimePeltier, m_offTimePeltier;
	volatile uint16_t m_onTimeMotor, m_offTimeMotor;
	volatile uint8_t m_dutyCycleHeater = 0;
	volatile uint8_t m_dutyCyclePeltier = 0;
	const float hysteresis = 0.2;  // °C

	// Variables for formatted string output
	char innerStr[6], extStr[6], motorPowStr[6], targetTempStr[6], targetRpmStr[6], currentRpmStr[6], outputHeaterStr[6];
	char message[MESSAGE_LENGTH];



	void setPeltierInHeatingMode()
	{
	  Serial.println("Peltier is going in Heating mode...");
	  setPeltierEnable(false);
	  digitalWrite(PELTIER_SWITCH_1_PIN, LOW);
	  digitalWrite(PELTIER_SWITCH_2_PIN, HIGH);
	}

	void setPeltierInCoolingMode()
	{
	  Serial.println("Peltier is going in Cooling mode...");
	  setPeltierEnable(false);
	  digitalWrite(PELTIER_SWITCH_1_PIN, HIGH);
	  digitalWrite(PELTIER_SWITCH_2_PIN, LOW);
	}

	void setPeltierEnable(bool en)
	{
	  if(en){
	    digitalWrite(PELTIER_EN_PIN, HIGH);
	  }else{
	    digitalWrite(PELTIER_EN_PIN, LOW);
	  }
	}

	void setup() {
	  Serial.begin(9600);
	  Serial.println("Starting...");

	  rtd1.begin(MAX31865_2WIRE); // 4-wire RTD
	  rtd2.begin(MAX31865_3WIRE); // 4-wire RTD

	  // Pin initialization
	  pinMode(PELTIER_PWM_PIN, OUTPUT);
	  pinMode(PELTIER_SWITCH_1_PIN, OUTPUT);
	  pinMode(PELTIER_SWITCH_2_PIN, OUTPUT);
	  pinMode(PELTIER_EN_PIN, OUTPUT);
	  pinMode(HEATER_PWM_PIN, OUTPUT);
	  pinMode(MOTOR_DIR_PIN, OUTPUT);
	  pinMode(MOTOR_PWM_PIN, OUTPUT);
	  Serial.println("Starting...");

	  //pinMode(MOTOR_FB_PIN, INPUT_PULLUP); // Set the interrupt pin as input
	  //attachInterrupt(digitalPinToInterrupt(MOTOR_FB_PIN), countRPM, FALLING); // Attach interrupt on falling edge

	  // Default pin states
	  setPeltierInCoolingMode();
	  setPeltierEnable(false);
	  digitalWrite(PELTIER_PWM_PIN, LOW);
	  digitalWrite(HEATER_PWM_PIN, LOW);
	  digitalWrite(MOTOR_DIR_PIN, HIGH);

	  m_rpmCount = 0;
	  m_dutyCycleHeater = 0;
	  m_dutyCyclePeltier = 0;
	  m_motorDutyCycle = 0;

	  readRTDTemperatureExt(rtd2, RREF, "RTD 2");
	  m_targetTemperature = m_currentTempExternal;
	  m_targetRpm = 0;

	  // Initialize PID controller
	  m_pidHeater.SetMode(AUTOMATIC);
	  m_pidHeater.SetOutputLimits(-100, 100);  // Output limits for both heating and cooling
	  //m_pidMotor.SetMode(AUTOMATIC);
	  //m_pidMotor.SetOutputLimits(0, 100);  // Limit output to PWM range

	  // Configure Timer 1 for 30kHz PWM (for both heater and peltier)
	  TCCR1A = 0;  
	  TCCR1B = (1 << WGM12) | (1 << CS10);  // CTC mode, no prescaler
	  OCR1A = 266;  // 30 kHz interrupt frequency
	  TIMSK1 |= (1 << OCIE1A);  // Enable compare match interrupt

	}



	void countRPM() {
	  m_rpmCount++; // Increment the count on each interrupt
	}


	void loop() {  


	 
	  readRTDTemperatureInner(rtd1, RREF, "RTD 1");
	  readRTDTemperatureExt(rtd2, RREF, "RTD 2");

    m_dutyCycleHeater = 0;
    setPeltierEnable(false);
	 /* m_pidHeater.Compute();


	  if(m_currentTempExternal > m_targetTemperature && m_outputHeater > 0){
	    m_outputHeater = 0;
	    setPeltierEnable(false);
	  }

	  if(m_currentTempExternal < m_targetTemperature && m_outputHeater < 0){
	    m_outputHeater = 0;
	    setPeltierEnable(false);
	  }

	  
	  

	  if(abs(m_currentTempExternal - m_targetTemperature) < hysteresis) {
	    // If within the hysteresis range, turn off both heater and Peltier
	    digitalWrite(HEATER_PWM_PIN, LOW);
	    digitalWrite(PELTIER_PWM_PIN, LOW);
	    setPeltierEnable(false);


	    m_dutyCycleHeater = 0;
	    m_dutyCyclePeltier = 100;
	  }
	  
	  
	  
	  //there is temp difference....
	  else {
	    // Adjust heater and peltier PWM based on the PID output
	    if (m_outputHeater >= 0) {
	      
	      
	      
	      /*if(m_outputHeater > 80){
		setPeltierInHeatingMode();
		setPeltierEnable(true);
		m_dutyCyclePeltier = 50;  // Invert the negative output
		updatePeltierPWMTiming();
	      }else{
		setPeltierInCoolingMode();
		setPeltierEnable(false);
	      }*/
/*
	      // Heating mode: positive PID output
	      m_dutyCycleHeater = constrain(m_outputHeater, 0, 100); // Limit to 0-100%
	      m_dutyCyclePeltier = 0;  // No cooling when heating
	    } else {
	      // Cooling mode: negative PID output
	      setPeltierEnable(true);
	      m_dutyCyclePeltier = 50;  // Invert the negative output
	      m_dutyCycleHeater = 0;  // No heating when cooling
	      
	    }

	    updatePeltierPWMTiming();
	    updateHeaterPWMTiming();

	    

	    dtostrf(m_outputHeater, 3, 1, outputHeaterStr);  // Convert double to string
	    snprintf(message, MESSAGE_LENGTH, "%s#%d#%d#%s", 
		   "pid", m_dutyCycleHeater, m_dutyCyclePeltier, outputHeaterStr);
	    Serial.println(message);

	  }


	    



	/*
	  m_rpmTimingEndTime = millis(); // Get the current time  
	  unsigned long rpmElapsedTime = m_rpmTimingEndTime - m_rpmTimingStartTime;
	  if (rpmElapsedTime >= RPM_UPDATE_MSECS) { // Update every 1 second
	    detachInterrupt(digitalPinToInterrupt(MOTOR_FB_PIN)); // Detach interrupt to prevent interference
	    unsigned long pulses = m_rpmCount; // Get the count
	    m_rpmCount = 0; // Reset the count
	    attachInterrupt(digitalPinToInterrupt(MOTOR_FB_PIN), countRPM, FALLING); // Reattach interrupt
	    m_currentRpm = (int)((float)pulses * 60000 / (rpmElapsedTime * 2)); // 2 pulses per revolution (rising and falling edge)
	    m_rpmTimingStartTime = millis(); // Reset the start time

	    m_pidMotor.Compute();
	    updateMotorPWMTiming();  
	  }*/

	 



	  Serial.println("GOGOGOGO");
	  delay(20);
	  handleSerialInput();

	  // Prepare serial message for custom format
	  dtostrf(m_currentTempInner, 3, 1, innerStr);  // Convert double to string
	  dtostrf(m_currentTempExternal, 3, 1, extStr);  // Convert double to string
	  dtostrf(m_motorDutyCycle, 3, 1, motorPowStr);  // Convert double to string
	  dtostrf(m_targetTemperature, 3, 1, targetTempStr);  // Convert double to string
	  dtostrf(m_targetRpm, 3, 1, targetRpmStr);  // Convert double to string
	  dtostrf(m_currentRpm, 3, 1, currentRpmStr);  // Convert double to string

	  snprintf(message, MESSAGE_LENGTH, "%s#%d#%s#%s#%s#%s#%s#%s", 
		   BUSBOARD_PREFIX, 2, innerStr, extStr, currentRpmStr, motorPowStr, targetTempStr, targetRpmStr);

	  Serial.println(message);
	}

	// Update heater PWM timing
	void updateHeaterPWMTiming() {
	  m_onTimeHeater = (m_dutyCycleHeater * 266) / 100;
	  m_offTimeHeater = 266 - m_onTimeHeater;
	}

	// Update peltier PWM timing
	void updatePeltierPWMTiming() {
	  m_onTimePeltier = (m_dutyCyclePeltier * 266) / 100;
	  m_offTimePeltier = 266 - m_onTimePeltier;
	}


	/*
	// Update motor PWM timing
	void updateMotorPWMTiming() {
	  m_onTimeMotor = (m_motorDutyCycle * 266) / 100;
	  m_offTimeMotor = 266 - m_onTimeMotor;
	}*/


	// Timer 1 interrupt service routine
	ISR(TIMER1_COMPA_vect) {
	  static bool isHighHeater = false, isHighPeltier = false, isHighMotor = false;
	  static uint16_t pwmCounterHeater = 0, pwmCounterPeltier = 0, pwmCounterMotor = 0;

	  // Heater PWM logic
	  if (isHighHeater) {
	    if (pwmCounterHeater >= m_onTimeHeater) {
	      digitalWrite(HEATER_PWM_PIN, LOW);
	      isHighHeater = false;
	      pwmCounterHeater = 0;
	    }
	  } else {
	    if (pwmCounterHeater >= m_offTimeHeater) {
	      digitalWrite(HEATER_PWM_PIN, HIGH);
	      isHighHeater = true;
	      pwmCounterHeater = 0;
	    }
	  }
	  pwmCounterHeater++;

	  // Peltier PWM logic
	  if (isHighPeltier) {
	    if (pwmCounterPeltier >= m_onTimePeltier) {
	      digitalWrite(PELTIER_PWM_PIN, LOW);
	      isHighPeltier = false;
	      pwmCounterPeltier = 0;
	    }
	  } else {
	    if (pwmCounterPeltier >= m_offTimePeltier) {
	      digitalWrite(PELTIER_PWM_PIN, HIGH);
	      isHighPeltier = true;
	      pwmCounterPeltier = 0;
	    }
	  }
	  pwmCounterPeltier++;

	  /*
	  // Motor PWM logic
	  if (isHighMotor) {
	    if (pwmCounterMotor >= m_onTimeMotor) {
	      digitalWrite(MOTOR_PWM_PIN, LOW);
	      isHighMotor = false;
	      pwmCounterMotor = 0;
	    }
	  } else {
	    if (pwmCounterMotor >= m_offTimeMotor) {
	      digitalWrite(MOTOR_PWM_PIN, HIGH);
	      isHighMotor = true;
	      pwmCounterMotor = 0;
	    }
	  }
	  pwmCounterMotor++;
	  */

	}

	// Function to read temperature and check faults for the RTD sensor
	void readRTDTemperatureExt(Adafruit_MAX31865 &rtdDev, float rref, const char *deviceName) {
	  uint16_t rtd = rtdDev.readRTD();
	  float temperature = rtdDev.temperature(100, rref);

	  uint8_t fault = rtdDev.readFault();
	  if (fault) {
	    rtdDev.clearFault();
	  } else {
	    m_currentTempExternal = temperature;  // Update global temperature variable
	  }
	}


	// Function to read temperature and check faults for the RTD sensor
	void readRTDTemperatureInner(Adafruit_MAX31865 &rtdDev, float rref, const char *deviceName) {
	  uint16_t rtd = rtdDev.readRTD();
	  float temperature = rtdDev.temperature(100, rref);

	  uint8_t fault = rtdDev.readFault();
	  if (fault) {
	    rtdDev.clearFault();
	    Serial.println("inner foult");
	  } else {
	    m_currentTempInner = temperature;  // Update global temperature variable
	  }
	}



	void handleSerialInput() {
	  if (Serial.available() > 0) {
	    String input = Serial.readStringUntil('\n');  // Read serial input until newline
	    input.trim();  // Remove any leading/trailing whitespace

	    // Check if the input starts with '>' and ends with '<'
	    if (input.startsWith(">") && input.endsWith("<")) {
	      // Remove '>' and '<'
	      input = input.substring(1, input.length() - 1);

	      // Check if the input is to set target temperature and RPM
	      int idx1 = input.indexOf('#');
	      int idx2 = input.indexOf('#', idx1 + 1);
	      int idx3 = input.indexOf('#', idx2 + 1);

	      if (idx1 != -1 && idx2 != -1 && idx3 != -1) {  // Format: "cellID#positionIdx#targetTemp#targetRPM"
		int positionIdx = input.substring(0, idx1).toInt();
		double targetTemp = input.substring(idx1 + 1, idx2).toDouble();
		int targetRPM = input.substring(idx2 + 1, idx3).toInt();
		int checksum = input.substring(idx3 + 1).toInt();

		if(checksum != positionIdx+(int)targetTemp+targetRPM){
		  Serial.print("Currupt data. checksum: ");
		  Serial.print(checksum);
		  Serial.print("  positionIdx+(int)targetTemp+targetRPM: ");
		  Serial.println(positionIdx+(int)targetTemp+targetRPM);
		  return;
		}

		// Update the corresponding variables
		m_targetTemperature = targetTemp;
		m_targetRpm = targetRPM;

		// Print confirmation of new settings
		Serial.print("Updated Parameters: positionIdx=");
		Serial.print(positionIdx);
		Serial.print(", targetTemp=");
		Serial.print(targetTemp);
		Serial.print(", targetRPM=");
		Serial.println(targetRPM);
	      }
	      else {
		Serial.println("Error: Invalid input format. Use '>cellID#positionIdx#targetTemp#targetRPM<'");
	      }
	    }
	    else if (input.startsWith("pid#")) {  // Check if input is for PID parameters
	      int pidIdx1 = input.indexOf('#', 4);  // Start after "pid#"
	      int pidIdx2 = input.indexOf('#', pidIdx1 + 1);

	      if (pidIdx1 != -1 && pidIdx2 != -1) {
		double kp = input.substring(4, pidIdx1).toDouble();
		double ki = input.substring(pidIdx1 + 1, pidIdx2).toDouble();
		double kd = input.substring(pidIdx2 + 1).toDouble();

		// Update PID parameters
		m_kp = kp;
		m_ki = ki;
		m_kd = kd;
		m_pidHeater.SetTunings(m_kp, m_ki, m_kd);  // Apply new tunings to PID controller

		// Print confirmation of updated PID parameters
		Serial.print("Updated PID Parameters: kp=");
		Serial.print(m_kp);
		Serial.print(", ki=");
		Serial.print(m_ki);
		Serial.print(", kd=");
		Serial.println(m_kd);
	      } else {
		Serial.println("Error: Invalid PID input format. Use 'pid#kp#ki#kd'");
	      }
	    }
	    else {
	      Serial.println("Error: Invalid input format. Use '>cellID#positionIdx#targetTemp#targetRPM<' or 'pid#kp#ki#kd'");
	    }
	  }
	}

