#include <EEPROM.h>
#include <PinChangeInterrupt.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>
#include <avr/wdt.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <PID_v1.h>
double setpoint = 50.0; // Desired setpoint
double input, output;
double Kp = 2.0; // Proportional gain
double Ki = 5.0; // Integral gain
double Kd = 1.0; // Derivative gain
PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

#define ADXL345_1_CS 7 // Chip select pin for the first ADXL345
#define ADXL345_2_CS 8 // Chip select pin for the second ADXL345

Adafruit_ADXL345_Unified accel1 = Adafruit_ADXL345_Unified(ADXL345_1_CS);
Adafruit_ADXL345_Unified accel2 = Adafruit_ADXL345_Unified(ADXL345_2_CS);

//PINLER----------

int cnt = 0;
volatile unsigned long count = 0;
const int interruptPin = 2;
unsigned long startTime;
unsigned long endTime;
int rpm;

SoftwareSerial mySerial(3, 4); // RX, TX

#define MESSAGE_LENGTH 50
#define BAUD 115200
char incomingMessage[MESSAGE_LENGTH]; 
char outgoingMessageBefore[MESSAGE_LENGTH];
char outgoingMessageAfter[MESSAGE_LENGTH];

char *BUSBOARD_PREFIX = "busboard000#";

void reboot() // Restarts program from beginning but does not reset the peripherals and registers
{
   Serial.println("reboot...");
   wdt_enable(WDTO_2S);
   exit(1);
}

void EEPROMWriteInt(int p_address, int p_value)
{
  byte lowByte = ((p_value >> 0) & 0xFF);
  byte highByte = ((p_value >> 8) & 0xFF);
  EEPROM.write(p_address, lowByte);
  EEPROM.write(p_address + 1, highByte);
}

unsigned int EEPROMReadInt(int p_address)
{
  byte lowByte = EEPROM.read(p_address);
  byte highByte = EEPROM.read(p_address + 1);
  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}

int calculateChecksum(const char* message) {
    int checksum = 0;
    for (int i = 0; message[i] != '\0'; ++i) {
        checksum += message[i];
    }
    return checksum;
}

void countPulse() {
  count++; // Increment the count on each interrupt
}


void setup()
{
  
  Serial.begin(BAUD);
  mySerial.begin(BAUD); // SoftwareSerial communication with Uno
  mySerial.listen();
  myPID.SetMode(AUTOMATIC); // Set PID to automatic mode

  randomSeed(analogRead(0)); // You can use any analog pin for seeding
  SPI.begin();

  // Initialize accelerometers
  if (!accel1.begin()) {
    Serial.println("Accelerometer 1 not found");
    while (1);
  }
  if (!accel2.begin()) {
    Serial.println("Accelerometer 2 not found");
    while (1);
  }
  pinMode(interruptPin, INPUT_PULLUP); // Set the interrupt pin as input
  attachInterrupt(digitalPinToInterrupt(interruptPin), countPulse, FALLING); // Attach interrupt on falling edge
  startTime = millis(); // Initialize the start time
  accel1.setRange(ADXL345_RANGE_16_G);
  accel2.setRange(ADXL345_RANGE_16_G);
  wdt_enable(WDTO_8S);
}


int targetRPM;
float targetTemp;

void loop()
{
  wdt_reset();
  //delay(150);
  sensors_event_t event1;
  accel1.getEvent(&event1);
  float xAccel1 = event1.acceleration.x;

  // Read acceleration from the second accelerometer
  sensors_event_t event2;
  accel2.getEvent(&event2);
  float xAccel2 = event2.acceleration.x;

  memset(incomingMessage, 0, MESSAGE_LENGTH);
  memset(outgoingMessageBefore, 0, MESSAGE_LENGTH);
  memset(outgoingMessageAfter, 0, MESSAGE_LENGTH);
  input = analogRead(A0); // Assuming input comes from analog pin A0

  // Compute PID output
  myPID.Compute();
  
  
  endTime = millis(); // Get the current time
  unsigned long elapsedTime = endTime - startTime; // Calculate the elapsed time
  
  if (elapsedTime >= 1000) { // Update every 1 second
    detachInterrupt(digitalPinToInterrupt(interruptPin)); // Detach interrupt to prevent interference
    unsigned long pulses = count; // Get the count
    count = 0; // Reset the count
    attachInterrupt(digitalPinToInterrupt(interruptPin), countPulse, FALLING); // Reattach interrupt
    
    // Calculate RPM
    rpm = (int)((float)pulses * 60000 / (elapsedTime * 2)); // 2 pulses per revolution (rising and falling edge)
    
   
    startTime = millis(); // Reset the start time
  }

  if (mySerial.available() > 0) {
      // Read data from Mega
      mySerial.readBytesUntil('\n', incomingMessage, MESSAGE_LENGTH);

      // Parse the incoming message
      char *ptr = strtok(incomingMessage, "#");
      targetTemp = atof(ptr);
      ptr = strtok(NULL, "#");
      targetRPM = atoi(ptr);
      Serial.print("message recieved from busboard: ");
      Serial.print(incomingMessage);
      Serial.print(" - temp: " );
      Serial.print(targetTemp);
      Serial.print(" - rpm: " );
      Serial.println(targetRPM);

      //delay(2000);
      
    }

  double amp = output;

  char innerStr[9];
  char extStr[9];
  char ampStr[9];
  char targetTempStr[9];

  dtostrf(xAccel1, 3, 1, innerStr);  // Convert double to string
  dtostrf(xAccel2, 3, 1, extStr);      // Convert double to string
  dtostrf(amp, 3, 1, ampStr);      // Convert double to string
  dtostrf(targetTemp, 3, 1, targetTempStr);      // Convert double to string

  snprintf(outgoingMessageBefore, MESSAGE_LENGTH, "%s#%d#%s#%s#%d#%s#%s#%d", "s25_111" , 1, innerStr, extStr, rpm, ampStr, targetTempStr, targetRPM);

  Serial.println(outgoingMessageBefore);

  int checksum = calculateChecksum(outgoingMessageBefore);
  
  snprintf(outgoingMessageAfter, MESSAGE_LENGTH, "%s#%d#%s#%s#%d#%s#%s#%d*%d\n", "s25_111" , 1, innerStr, extStr, rpm, ampStr, targetTempStr, targetRPM, checksum);
  Serial.println(outgoingMessageAfter);

  mySerial.println(outgoingMessageAfter);
  delay(10);
  cnt++;
  cnt = cnt%10;
  

}