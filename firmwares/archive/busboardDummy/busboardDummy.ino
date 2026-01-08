#include <SoftwareSerial.h>

#define MESSAGE_LENGTH 64
#define BAUD 115200
char incomingMessage[MESSAGE_LENGTH];
char incomingMessageUNO1[MESSAGE_LENGTH];
char incomingMessageUNO2[MESSAGE_LENGTH];
char incomingMessageUNO3[MESSAGE_LENGTH];
char incomingMessageUNO4[MESSAGE_LENGTH];
char incomingMessageUNO5[MESSAGE_LENGTH];
char incomingMessageUNO6[MESSAGE_LENGTH];
char incomingMessageUNO7[MESSAGE_LENGTH];
char incomingMessageUNO8[MESSAGE_LENGTH];
char incomingMessageUNO9[MESSAGE_LENGTH];
char incomingMessageUNO10[MESSAGE_LENGTH];


char outgoingMessage[MESSAGE_LENGTH];


int calculateChecksum(const char* message) {
    int checksum = 0;
    for (int i = 0; message[i] != '\0'; ++i) {
        checksum += message[i];
    }
    return checksum;
}

void parseMessageAndCRC(char *input, char *message, int *crc) {
    char *token = strtok(input, "*"); // Split the input string by '*'
    if (token != NULL) {
        strcpy(message, token); // Copy the message part
        token = strtok(NULL, "*"); // Get the next token, which is CRC
        if (token != NULL) {
            sscanf(token, "%d", crc); // Parse CRC as integer
        }
    }

}


void parseMessage(char *input, int *positionIndex, double *targetTmp, int *targetRpm) {
  char* token = strtok(const_cast<char*>(input), "#");

    // Parse position index
    if (token != NULL) {
        *positionIndex = atoi(token);
        token = strtok(NULL, "#");
    }
    double tmp;
    // Parse temperature
    if (token != NULL) {
        tmp = atof(token);
        *targetTmp = tmp;
        token = strtok(NULL, "#");
    }

    // Parse target RPM
    if (token != NULL) {
        *targetRpm = atoi(token);
        token = strtok(NULL, "#");
    }

    int checksumRec;
    // Parse target RPM
    if (token != NULL) {
        checksumRec = atoi(token);
    }
    int checksumCalculated = *positionIndex + (int)(tmp) + *targetRpm;
    if(checksumRec != checksumCalculated){
      *positionIndex = -1;
    }
    
}

void setup() {
  Serial.begin(BAUD);     // For debugging
  Serial.println(BUSBOARD_HELLO);
}

char serial[10], cell1serial[10];
int positionIndex, targetRpm1, targetRpm2, targetRpm3, targetRpm4, targetRpm5;
int targetRpm6, targetRpm7, targetRpm8, targetRpm9, targetRpm10;
double targetTmp1, targetTmp2, targetTmp3, targetTmp4, targetTmp5, targetTmp6;
double targetTmp7, targetTmp8, targetTmp9, targetTmp10;

int targetRpm;
double targetTmp;
float innerTemp1, extTemp1, amp1;
int rpm1;
int incomingChecksum, calculatedChecksum;

#define BUSBOARD_PREFIX "bb_000#"
#define BUSBOARD_HELLO "bb_000#HELLO"

void loop() {

Serial.flush();
 /* cell1.flush();
  cell2.flush();
  cell3.flush();
  cell4.flush();*/
  memset(incomingMessageUNO1, 0, MESSAGE_LENGTH);
  memset(incomingMessageUNO2, 0, MESSAGE_LENGTH);
  memset(incomingMessageUNO3, 0, MESSAGE_LENGTH);
  memset(incomingMessageUNO4, 0, MESSAGE_LENGTH);
  memset(incomingMessageUNO5, 0, MESSAGE_LENGTH);
  memset(incomingMessageUNO6, 0, MESSAGE_LENGTH);
  memset(incomingMessageUNO7, 0, MESSAGE_LENGTH);
  memset(incomingMessageUNO8, 0, MESSAGE_LENGTH);
  memset(incomingMessageUNO9, 0, MESSAGE_LENGTH);
  memset(incomingMessageUNO10, 0, MESSAGE_LENGTH);

  calculatedChecksum = 0;
  incomingChecksum = 0;

  float amp = 20.0 + (analogRead(A6))%8;
  int rpm = 11 + analogRead(A3);
  float xAccel1 = 13.0 + (analogRead(A4))%5;
  float xAccel2 = 15.0 + (analogRead(A5))%4;
  float flowRateLpm = 1.0 + (analogRead(A0)) % 5;
  float flowTemp = 23.0 + (analogRead(A1)) % 7;

  char innerStr[9];
  char extStr[9];
  char ampStr[9];
  char targetTempStr[9];

  dtostrf(xAccel1, 3, 1, innerStr);  // Convert double to string
  dtostrf(xAccel2, 3, 1, extStr);      // Convert double to string
  dtostrf(amp, 3, 1, ampStr);      // Convert double to string
  dtostrf(targetTmp1, 3, 1, targetTempStr);      // Convert double to string

  snprintf(incomingMessageUNO1, MESSAGE_LENGTH, "%s#%d#%s#%s#%d#%s#%s#%d#%.2f#%.1f", "s25_111" , 1, innerStr, extStr, rpm, ampStr, targetTempStr, targetRpm1, flowRateLpm, flowTemp);

  Serial.print(BUSBOARD_PREFIX);
  Serial.println(incomingMessageUNO1);

 // delay(20);
  amp = 20.0 + (analogRead(A6))%8;
  rpm = 11 + analogRead(A3);
  xAccel1 = 13.0 + (analogRead(A4))%5;
  xAccel2 = 15.0 + (analogRead(A5))%4;


  dtostrf(xAccel1, 3, 1, innerStr);  // Convert double to string
  dtostrf(xAccel2, 3, 1, extStr);      // Convert double to string
  dtostrf(amp, 3, 1, ampStr);      // Convert double to string
  dtostrf(targetTmp2, 3, 1, targetTempStr);      // Convert double to string

  snprintf(incomingMessageUNO1, MESSAGE_LENGTH, "%s#%d#%s#%s#%d#%s#%s#%d#%.2f#%.1f", "s25_222" , 2, innerStr, extStr, rpm, ampStr, targetTempStr, targetRpm2, flowRateLpm, flowTemp);

  Serial.print(BUSBOARD_PREFIX);
  Serial.println(incomingMessageUNO1);

  //delay(20);
  amp = 20.0 + (analogRead(A6))%8;
  rpm = 11 + analogRead(A3);
  xAccel1 = 13.0 + (analogRead(A4))%5;
  xAccel2 = 15.0 + (analogRead(A5))%4;


  dtostrf(xAccel1, 3, 1, innerStr);  // Convert double to string
  dtostrf(xAccel2, 3, 1, extStr);      // Convert double to string
  dtostrf(amp, 3, 1, ampStr);      // Convert double to string
  dtostrf(targetTmp3, 3, 1, targetTempStr);      // Convert double to string

  snprintf(incomingMessageUNO1, MESSAGE_LENGTH, "%s#%d#%s#%s#%d#%s#%s#%d#%.2f#%.1f", "s25_333" , 3, innerStr, extStr, rpm, ampStr, targetTempStr, targetRpm3, flowRateLpm, flowTemp);

  Serial.print(BUSBOARD_PREFIX);
  Serial.println(incomingMessageUNO1);




  //delay(20);
  amp = 20.0 + (analogRead(A6))%8;
  rpm = 11 + analogRead(A3);
  xAccel1 = 13.0 + (analogRead(A4))%5;
  xAccel2 = 15.0 + (analogRead(A5))%4;


  dtostrf(xAccel1, 3, 1, innerStr);  // Convert double to string
  dtostrf(xAccel2, 3, 1, extStr);      // Convert double to string
  dtostrf(amp, 3, 1, ampStr);      // Convert double to string
  dtostrf(targetTmp4, 3, 1, targetTempStr);      // Convert double to string

  snprintf(incomingMessageUNO1, MESSAGE_LENGTH, "%s#%d#%s#%s#%d#%s#%s#%d#%.2f#%.1f", "s25_444" , 4, innerStr, extStr, rpm, ampStr, targetTempStr, targetRpm4, flowRateLpm, flowTemp);

  Serial.print(BUSBOARD_PREFIX);
  Serial.println(incomingMessageUNO1);
  //delay(20);




  amp = 20.0 + (analogRead(A6))%8;
  rpm = 11 + analogRead(A3);
  xAccel1 = 13.0 + (analogRead(A4))%5;
  xAccel2 = 15.0 + (analogRead(A5))%4;


  dtostrf(xAccel1, 3, 1, innerStr);  // Convert double to string
  dtostrf(xAccel2, 3, 1, extStr);      // Convert double to string
  dtostrf(amp, 3, 1, ampStr);      // Convert double to string
  dtostrf(targetTmp5, 3, 1, targetTempStr);      // Convert double to string

  snprintf(incomingMessageUNO1, MESSAGE_LENGTH, "%s#%d#%s#%s#%d#%s#%s#%d#%.2f#%.1f", "s25_555" , 5, innerStr, extStr, rpm, ampStr, targetTempStr, targetRpm5, flowRateLpm, flowTemp);

  Serial.print(BUSBOARD_PREFIX);
  Serial.println(incomingMessageUNO1);
  //delay(20);


  amp = 20.0 + (analogRead(A6))%8;
  rpm = 11 + analogRead(A3);
  xAccel1 = 13.0 + (analogRead(A4))%5;
  xAccel2 = 15.0 + (analogRead(A5))%4;


  dtostrf(xAccel1, 3, 1, innerStr);  // Convert double to string
  dtostrf(xAccel2, 3, 1, extStr);      // Convert double to string
  dtostrf(amp, 3, 1, ampStr);      // Convert double to string
  dtostrf(targetTmp6, 3, 1, targetTempStr);      // Convert double to string

  snprintf(incomingMessageUNO1, MESSAGE_LENGTH, "%s#%d#%s#%s#%d#%s#%s#%d#%.2f#%.1f", "s25_666" , 6, innerStr, extStr, rpm, ampStr, targetTempStr, targetRpm6, flowRateLpm, flowTemp);

  Serial.print(BUSBOARD_PREFIX);
  Serial.println(incomingMessageUNO1);
  //delay(20);


  amp = 20.0 + (analogRead(A6))%8;
  rpm = 11 + analogRead(A3);
  xAccel1 = 13.0 + (analogRead(A4))%5;
  xAccel2 = 15.0 + (analogRead(A5))%4;


  dtostrf(xAccel1, 3, 1, innerStr);  // Convert double to string
  dtostrf(xAccel2, 3, 1, extStr);      // Convert double to string
  dtostrf(amp, 3, 1, ampStr);      // Convert double to string
  dtostrf(targetTmp7, 3, 1, targetTempStr);      // Convert double to string

  snprintf(incomingMessageUNO1, MESSAGE_LENGTH, "%s#%d#%s#%s#%d#%s#%s#%d#%.2f#%.1f", "s25_777" , 7, innerStr, extStr, rpm, ampStr, targetTempStr, targetRpm7, flowRateLpm, flowTemp);

  Serial.print(BUSBOARD_PREFIX);
  Serial.println(incomingMessageUNO1);
  //delay(20);


  amp = 20.0 + (analogRead(A6))%8;
  rpm = 11 + analogRead(A3);
  xAccel1 = 13.0 + (analogRead(A4))%5;
  xAccel2 = 15.0 + (analogRead(A5))%4;


  dtostrf(xAccel1, 3, 1, innerStr);  // Convert double to string
  dtostrf(xAccel2, 3, 1, extStr);      // Convert double to string
  dtostrf(amp, 3, 1, ampStr);      // Convert double to string
  dtostrf(targetTmp8, 3, 1, targetTempStr);      // Convert double to string

  snprintf(incomingMessageUNO1, MESSAGE_LENGTH, "%s#%d#%s#%s#%d#%s#%s#%d#%.2f#%.1f", "s25_888" , 8, innerStr, extStr, rpm, ampStr, targetTempStr, targetRpm8, flowRateLpm, flowTemp);

  Serial.print(BUSBOARD_PREFIX);
  Serial.println(incomingMessageUNO1);
 // delay(20);


  amp = 20.0 + (analogRead(A6))%8;
  rpm = 11 + analogRead(A3);
  xAccel1 = 13.0 + (analogRead(A4))%5;
  xAccel2 = 15.0 + (analogRead(A5))%4;


  dtostrf(xAccel1, 3, 1, innerStr);  // Convert double to string
  dtostrf(xAccel2, 3, 1, extStr);      // Convert double to string
  dtostrf(amp, 3, 1, ampStr);      // Convert double to string
  dtostrf(targetTmp9, 3, 1, targetTempStr);      // Convert double to string

  snprintf(incomingMessageUNO1, MESSAGE_LENGTH, "%s#%d#%s#%s#%d#%s#%s#%d#%.2f#%.1f", "s25_999" , 9, innerStr, extStr, rpm, ampStr, targetTempStr, targetRpm9, flowRateLpm, flowTemp);

  Serial.print(BUSBOARD_PREFIX);
  Serial.println(incomingMessageUNO1);
  //delay(20);


  amp = 20.0 + (analogRead(A6))%8;
  rpm = 11 + analogRead(A3);
  xAccel1 = 13.0 + (analogRead(A4))%5;
  xAccel2 = 15.0 + (analogRead(A5))%4;


  dtostrf(xAccel1, 3, 1, innerStr);  // Convert double to string
  dtostrf(xAccel2, 3, 1, extStr);      // Convert double to string
  dtostrf(amp, 3, 1, ampStr);      // Convert double to string
  dtostrf(targetTmp10, 3, 1, targetTempStr);      // Convert double to string

  snprintf(incomingMessageUNO1, MESSAGE_LENGTH, "%s#%d#%s#%s#%d#%s#%s#%d#%.2f#%.1f", "s25_101010" , 10, innerStr, extStr, rpm, ampStr, targetTempStr, targetRpm10, flowRateLpm, flowTemp);

  Serial.print(BUSBOARD_PREFIX);
  Serial.println(incomingMessageUNO1);
  //delay(20);


  

  Serial.println("GO");
    delay(20);

  // PC'den gelen update'ler alınıyor
  while (Serial.available() > 0) {
        Serial.print("serial available------------------ ");

    // Read data from Serial
    Serial.readBytes(incomingMessage, MESSAGE_LENGTH);
    //Serial.readBytesUntil('<', incomingMessage, MESSAGE_LENGTH);

    char incomingCopy[MESSAGE_LENGTH];
    strcpy(incomingCopy, incomingMessage);
    

   // Serial.println(incomingCopy);
    
    
    char tempInput[strlen(incomingCopy) + 1];
    strcpy(tempInput, incomingCopy);

    char *startToken = strstr(tempInput, ">");
      while (startToken != NULL) {
        char *endToken = strstr(startToken, "<");
        if (endToken == NULL) {
            // If no end token is found, break out of the loop
            break;
        }

        char substring[endToken - startToken + 1];
        strncpy(substring, startToken + 1, endToken - startToken - 1);
        substring[endToken - startToken - 1] = '\0';
        //Serial.println(substring);

        int positionIndexToUpdate;
        double tTmp;
        int tRpm;

        // Parse each token as a separate message
        parseMessage(substring, &positionIndexToUpdate, &tTmp, &tRpm);

        // Process the parsed data as needed
        /*Serial.print("Position Index: ");
        Serial.println(positionIndexToUpdate);
        Serial.print("Target Temperature: ");
        Serial.println(tTmp);
        Serial.print("Target RPM: ");
        Serial.println(tRpm);*/

        switch (positionIndexToUpdate) {
          case 1:
              targetRpm1 = tRpm;
              targetTmp1 = tTmp;
              break;
          case 2:
              targetRpm2 = tRpm;
              targetTmp2 = tTmp;
              break;
          case 3:
              targetRpm3 = tRpm;
              targetTmp3 = tTmp;
              break;
          case 4:
              targetRpm4 = tRpm;
              targetTmp4 = tTmp;
              break;
          case 5:
              targetRpm5 = tRpm;
              targetTmp5 = tTmp;
              break;
          case 6:
              targetRpm6 = tRpm;
              targetTmp6 = tTmp;
              break;
          case 7:
              targetRpm7 = tRpm;
              targetTmp7 = tTmp;
              break;
          case 8:
              targetRpm8 = tRpm;
              targetTmp8 = tTmp;
              break;
          case 9:
              targetRpm9 = tRpm;
              targetTmp9 = tTmp;
              break;
          case 10:
              targetRpm10 = tRpm;
              targetTmp10 = tTmp;
              break;
          default:
              break;
        }

        // Move to the next token
        startToken = strstr(endToken, ">");

    }
  }

  //Serial.flush();

  //delay(10);
}
