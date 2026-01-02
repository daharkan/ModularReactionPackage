#include <EEPROM.h>
#include <PinChangeInterrupt.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>
#include <avr/wdt.h>
#include <SoftwareSerial.h>

//PINLER----------

#define BAUD 115200
#define MESSAGE_LENGTH 60
char incomingMessage[MESSAGE_LENGTH]; 
char outgoingMessage[MESSAGE_LENGTH];
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


String dataString;
int *rpms = new int[10]{};
float *temps = new float[10]{};
int cnt = 0;

void setup()
{
  
  Serial.begin(BAUD);
  randomSeed(analogRead(0)); // You can use any analog pin for seeding


  wdt_enable(WDTO_8S);
}

void loop()
{
  wdt_reset();
  //delay(150);
  if (Serial.available() >= 4) {
      // Read the incoming message
      dataString = Serial.readStringUntil('\n');
      
      char cellID[20]; // Adjust the size according to your maximum expected cellID length
      int positionIdx = 0;
      float targetTemp = 0.0;
      int targetRPM = 0;

      char* ptr = dataString.c_str();
      char* endPtr;

      // Extract cellID
      endPtr = strchr(ptr, '#');
      if (endPtr != NULL) {
          strncpy(cellID, ptr, endPtr - ptr);
          cellID[endPtr - ptr] = '\0';

          // Move ptr to the next character after the delimiter
          ptr = endPtr + 1;

          // Extract positionIdx
          positionIdx = atoi(ptr);

          // Move ptr to the next character after the delimiter
          ptr = strchr(ptr, '#') + 1;

          // Extract targetTemp
          targetTemp = atof(ptr);

          // Move ptr to the next character after the delimiter
          ptr = strchr(ptr, '#') + 1;

          // Extract targetRPM
          targetRPM = atoi(ptr);
          rpms[positionIdx-1] = targetRPM;
          temps[positionIdx-1] = targetTemp;
      }


      
  }
    

    double inner = 25.0 + rand()%15 + (rand()%100)/100.0;
    double amp = 5.0 + rand()%13 + + (rand()%100)/100.0;



    char innerStr[10];
    char extStr[10];
    char ampStr[10];

    dtostrf(inner, 5, 2, innerStr);  // Convert double to string
    dtostrf(temps[cnt], 5, 2, extStr);      // Convert double to string
    dtostrf(amp, 5, 2, ampStr);      // Convert double to string

    snprintf(outgoingMessage, MESSAGE_LENGTH, "%s%s%d#%d#%s#%s#%d#%s\r\n", BUSBOARD_PREFIX, "s25_", 111 * (cnt+1), cnt+1, innerStr, extStr, rpms[cnt], ampStr);

    Serial.write(outgoingMessage);
    delay(50);
    cnt++;
    cnt = cnt%10;
  

}