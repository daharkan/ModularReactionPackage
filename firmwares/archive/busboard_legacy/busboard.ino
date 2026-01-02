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

SoftwareSerial cell1(10, 47); // RX, TX
SoftwareSerial cell2(11, 46); // RX, TX
SoftwareSerial cell3(12, 49); // RX, TX
SoftwareSerial cell4(13, 48); // RX, TX
SoftwareSerial cell5(50, 22); // RX, TX
SoftwareSerial cell6(51, 23); // RX, TX
SoftwareSerial cell7(52, 24); // RX, TX
SoftwareSerial cell8(53, 25); // RX, TX
SoftwareSerial cell9(62, 26); // RX, TX
SoftwareSerial cell10(63, 27); // RX, TX

int toggleCounter;


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

void setup() {
  toggleCounter = 0;
  Serial.begin(BAUD);     // For debugging
  
  cell1.begin(BAUD);   // SoftwareSerial communication with Uno
  cell2.begin(BAUD);   // SoftwareSerial communication with Uno
  cell3.begin(BAUD);   // SoftwareSerial communication with Uno
  cell4.begin(BAUD);   // SoftwareSerial communication with Uno
  cell5.begin(BAUD);   // SoftwareSerial communication with Uno
  cell6.begin(BAUD);   // SoftwareSerial communication with Uno
  cell7.begin(BAUD);   // SoftwareSerial communication with Uno
  cell8.begin(BAUD);   // SoftwareSerial communication with Uno
  cell9.begin(BAUD);   // SoftwareSerial communication with Uno
  cell10.begin(BAUD);   // SoftwareSerial communication with Uno

}

char serial[10], cell1serial[10];
int positionIndex, targetRpm;
double targetTmp;
float innerTemp1, extTemp1, amp1;
int rpm1;
int incomingChecksum, calculatedChecksum;

#define BUSBOARD_PREFIX "bb_000#"

void loop() {

  toggleCounter++;
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
switch(toggleCounter){
  case 1:
    cell1.listen();
    delay(10);
    if (cell1.available() > 0) {
      // Read data from Uno
      cell1.readBytesUntil('\n', incomingMessageUNO1, MESSAGE_LENGTH);
      //incomingMessageUNO1[MESSAGE_LENGTH - 1] = '\0';
      
      parseMessageAndCRC(incomingMessageUNO1, incomingMessageUNO1, &incomingChecksum);
      calculatedChecksum = calculateChecksum(incomingMessageUNO1);

      if (incomingChecksum != calculatedChecksum || strlen(incomingMessageUNO1) < 5) {
        return;
      }
      sscanf(incomingMessageUNO1, "%9[^#]#%d#%f#%f#%d#%f#%d\n", cell1serial, &positionIndex, &innerTemp1, &extTemp1, &amp1, &rpm1, &targetTmp, &targetRpm);

      Serial.print(BUSBOARD_PREFIX);
      Serial.println(incomingMessageUNO1);
      
      cell1.flush();

    }

    break;
  
  case 2:    
    cell2.listen();
    delay(10);
    if (cell2.available() > 0) {
      // Read data from Uno
      cell2.readBytesUntil('\n', incomingMessageUNO2, MESSAGE_LENGTH);
      //incomingMessageUNO1[MESSAGE_LENGTH - 1] = '\0';
      
      parseMessageAndCRC(incomingMessageUNO2, incomingMessageUNO2, &incomingChecksum);
      calculatedChecksum = calculateChecksum(incomingMessageUNO2);

      if (incomingChecksum != calculatedChecksum || strlen(incomingMessageUNO2) < 5) {
        return;
      }
      sscanf(incomingMessageUNO2, "%9[^#]#%d#%f#%f#%d#%f#%d\n", cell1serial, &positionIndex, &innerTemp1, &extTemp1, &amp1, &rpm1, &targetTmp, &targetRpm);

      Serial.print(BUSBOARD_PREFIX);
      Serial.println(incomingMessageUNO2);
      
      cell2.flush();

    }
    break;

  case 3:    
    cell3.listen();
    delay(10);
    if (cell3.available() > 0) {
      // Read data from Uno
      cell3.readBytesUntil('\n', incomingMessageUNO3, MESSAGE_LENGTH);
      
      parseMessageAndCRC(incomingMessageUNO3, incomingMessageUNO3, &incomingChecksum);
      calculatedChecksum = calculateChecksum(incomingMessageUNO3);

      if (incomingChecksum != calculatedChecksum || strlen(incomingMessageUNO3) < 5) {
        return;
      }
      sscanf(incomingMessageUNO3, "%9[^#]#%d#%f#%f#%d#%f#%d\n", cell1serial, &positionIndex, &innerTemp1, &extTemp1, &amp1, &rpm1, &targetTmp, &targetRpm);

      Serial.print(BUSBOARD_PREFIX);
      Serial.println(incomingMessageUNO3);
      
      cell3.flush();

    }
    break;

  case 4:    
    cell4.listen();
    delay(10);
    if (cell4.available() > 0) {
      // Read data from Uno
      cell4.readBytesUntil('\n', incomingMessageUNO4, MESSAGE_LENGTH);
     
      parseMessageAndCRC(incomingMessageUNO4, incomingMessageUNO4, &incomingChecksum);
      calculatedChecksum = calculateChecksum(incomingMessageUNO4);

      if (incomingChecksum != calculatedChecksum || strlen(incomingMessageUNO4) < 5) {
        return;
      }
      sscanf(incomingMessageUNO4, "%9[^#]#%d#%f#%f#%d#%f#%d\n", cell1serial, &positionIndex, &innerTemp1, &extTemp1, &amp1, &rpm1, &targetTmp, &targetRpm);

      Serial.print(BUSBOARD_PREFIX);
      Serial.println(incomingMessageUNO4);
      
      cell4.flush();

    }

    break;

  
  case 5:    
    cell5.listen();
    delay(10);
    if (cell5.available() > 0) {
      // Read data from Uno
      cell5.readBytesUntil('\n', incomingMessageUNO5, MESSAGE_LENGTH);
     
      parseMessageAndCRC(incomingMessageUNO5, incomingMessageUNO5, &incomingChecksum);
      calculatedChecksum = calculateChecksum(incomingMessageUNO5);

      if (incomingChecksum != calculatedChecksum || strlen(incomingMessageUNO5) < 5) {
        return;
      }
      sscanf(incomingMessageUNO5, "%9[^#]#%d#%f#%f#%d#%f#%d\n", cell1serial, &positionIndex, &innerTemp1, &extTemp1, &amp1, &rpm1, &targetTmp, &targetRpm);

      Serial.print(BUSBOARD_PREFIX);
      Serial.println(incomingMessageUNO5);
      
      cell5.flush();

    }

    break;


  case 6:    
    cell6.listen();
    delay(10);
    if (cell6.available() > 0) {
      // Read data from Uno
      cell6.readBytesUntil('\n', incomingMessageUNO6, MESSAGE_LENGTH);
     
      parseMessageAndCRC(incomingMessageUNO6, incomingMessageUNO6, &incomingChecksum);
      calculatedChecksum = calculateChecksum(incomingMessageUNO6);

      if (incomingChecksum != calculatedChecksum || strlen(incomingMessageUNO6) < 5) {
        return;
      }
      sscanf(incomingMessageUNO6, "%9[^#]#%d#%f#%f#%d#%f#%d\n", cell1serial, &positionIndex, &innerTemp1, &extTemp1, &amp1, &rpm1, &targetTmp, &targetRpm);

      Serial.print(BUSBOARD_PREFIX);
      Serial.println(incomingMessageUNO6);
      
      cell6.flush();

    }

    break;


    case 7:    
    cell7.listen();
    delay(10);
    if (cell7.available() > 0) {
      // Read data from Uno
      cell7.readBytesUntil('\n', incomingMessageUNO7, MESSAGE_LENGTH);
     
      parseMessageAndCRC(incomingMessageUNO7, incomingMessageUNO7, &incomingChecksum);
      calculatedChecksum = calculateChecksum(incomingMessageUNO7);

      if (incomingChecksum != calculatedChecksum || strlen(incomingMessageUNO7) < 5) {
        return;
      }
      sscanf(incomingMessageUNO7, "%9[^#]#%d#%f#%f#%d#%f#%d\n", cell1serial, &positionIndex, &innerTemp1, &extTemp1, &amp1, &rpm1, &targetTmp, &targetRpm);

      Serial.print(BUSBOARD_PREFIX);
      Serial.println(incomingMessageUNO7);
      
      cell7.flush();

    }

    break;


    case 8:    
    cell8.listen();
    delay(10);
    if (cell8.available() > 0) {
      // Read data from Uno
      cell8.readBytesUntil('\n', incomingMessageUNO8, MESSAGE_LENGTH);
     
      parseMessageAndCRC(incomingMessageUNO8, incomingMessageUNO8, &incomingChecksum);
      calculatedChecksum = calculateChecksum(incomingMessageUNO8);

      if (incomingChecksum != calculatedChecksum || strlen(incomingMessageUNO8) < 5) {
        return;
      }
      sscanf(incomingMessageUNO8, "%9[^#]#%d#%f#%f#%d#%f#%d\n", cell1serial, &positionIndex, &innerTemp1, &extTemp1, &amp1, &rpm1, &targetTmp, &targetRpm);

      Serial.print(BUSBOARD_PREFIX);
      Serial.println(incomingMessageUNO8);
      
      cell8.flush();

    }

    break;


    case 9:    
    cell9.listen();
    delay(10);
    if (cell9.available() > 0) {
      // Read data from Uno
      cell9.readBytesUntil('\n', incomingMessageUNO9, MESSAGE_LENGTH);
     
      parseMessageAndCRC(incomingMessageUNO9, incomingMessageUNO9, &incomingChecksum);
      calculatedChecksum = calculateChecksum(incomingMessageUNO9);

      if (incomingChecksum != calculatedChecksum || strlen(incomingMessageUNO9) < 5) {
        return;
      }
      sscanf(incomingMessageUNO9, "%9[^#]#%d#%f#%f#%d#%f#%d\n", cell1serial, &positionIndex, &innerTemp1, &extTemp1, &amp1, &rpm1, &targetTmp, &targetRpm);

      Serial.print(BUSBOARD_PREFIX);
      Serial.println(incomingMessageUNO9);
      
      cell9.flush();

    }

    break;


    case 10:    
    cell10.listen();
    delay(10);
    if (cell10.available() > 0) {
      // Read data from Uno
      cell10.readBytesUntil('\n', incomingMessageUNO10, MESSAGE_LENGTH);
     
      parseMessageAndCRC(incomingMessageUNO10, incomingMessageUNO10, &incomingChecksum);
      calculatedChecksum = calculateChecksum(incomingMessageUNO10);

      if (incomingChecksum != calculatedChecksum || strlen(incomingMessageUNO10) < 5) {
        return;
      }
      sscanf(incomingMessageUNO10, "%9[^#]#%d#%f#%f#%d#%f#%d\n", cell1serial, &positionIndex, &innerTemp1, &extTemp1, &amp1, &rpm1, &targetTmp, &targetRpm);

      Serial.print(BUSBOARD_PREFIX);
      Serial.println(incomingMessageUNO10);
      
      cell10.flush();

    }

    break;
}
 
  if(toggleCounter > 10){
    toggleCounter = 0;
  }




  int positionIndexToUpdate = -1;
  // PC'den gelen update'ler alınıyor
  if (Serial.available() > 0) {
    // Read data from Serial
    Serial.readBytesUntil('\n', incomingMessage, MESSAGE_LENGTH);

    char incomingCopy[MESSAGE_LENGTH];
    strcpy(incomingCopy, incomingMessage);


    char* token = strtok(const_cast<char*>(incomingCopy), "#");

    // Parse position index
    if (token != NULL) {
        positionIndexToUpdate = atoi(token);
        token = strtok(NULL, "#");
    }

    // Parse temperature
    if (token != NULL) {
        targetTmp = atof(token);
        token = strtok(NULL, "#");
    }

    // Parse target RPM
    if (token != NULL) {
        targetRpm = atoi(token);
    }

  }


  //cell 1 ise PC'den gelen update
  if(positionIndexToUpdate == 1){
    positionIndexToUpdate = -1;

  // Continuously stream data via serial
    char targetTempStr[10];
    dtostrf(targetTmp, 5, 2, targetTempStr);      // Convert double to string

    snprintf(outgoingMessage, MESSAGE_LENGTH, "%s#%d\r\n", targetTempStr, targetRpm);
    // Send data to Uno
    cell1.print(outgoingMessage);
    
    targetRpm = 0;
    targetTmp = 0;
  }
  

    //cell 2 ise PC'den gelen update
  if(positionIndexToUpdate == 2){
    positionIndexToUpdate = -1;

  // Continuously stream data via serial
    char targetTempStr[10];
    dtostrf(targetTmp, 5, 2, targetTempStr);      // Convert double to string

    snprintf(outgoingMessage, MESSAGE_LENGTH, "%s#%d\r\n", targetTempStr, targetRpm);
    // Send data to Uno
    cell2.print(outgoingMessage);

    targetRpm = 0;
    targetTmp = 0;
  }

    //cell 3 ise PC'den gelen update
  if(positionIndexToUpdate == 3){
    positionIndexToUpdate = -1;

  // Continuously stream data via serial
    char targetTempStr[10];
    dtostrf(targetTmp, 5, 2, targetTempStr);      // Convert double to string

    snprintf(outgoingMessage, MESSAGE_LENGTH, "%s#%d\r\n", targetTempStr, targetRpm);
    // Send data to Uno
    cell3.print(outgoingMessage);
    
    targetRpm = 0;
    targetTmp = 0;
  }
  


  if(positionIndexToUpdate == 4){
    positionIndexToUpdate = -1;

  // Continuously stream data via serial
    char targetTempStr[10];
    dtostrf(targetTmp, 5, 2, targetTempStr);      // Convert double to string

    snprintf(outgoingMessage, MESSAGE_LENGTH, "%s#%d\r\n", targetTempStr, targetRpm);
    // Send data to Uno
    cell4.print(outgoingMessage);
    
    targetRpm = 0;
    targetTmp = 0;
  }



    //cell 5 ise PC'den gelen update
  if(positionIndexToUpdate == 5){
    positionIndexToUpdate = -1;

  // Continuously stream data via serial
    char targetTempStr[10];
    dtostrf(targetTmp, 5, 2, targetTempStr);      // Convert double to string

    snprintf(outgoingMessage, MESSAGE_LENGTH, "%s#%d\r\n", targetTempStr, targetRpm);
    // Send data to Uno
    cell5.print(outgoingMessage);
    
    targetRpm = 0;
    targetTmp = 0;
  }

if(positionIndexToUpdate == 6){
    positionIndexToUpdate = -1;

  // Continuously stream data via serial
    char targetTempStr[10];
    dtostrf(targetTmp, 5, 2, targetTempStr);      // Convert double to string

    snprintf(outgoingMessage, MESSAGE_LENGTH, "%s#%d\r\n", targetTempStr, targetRpm);
    // Send data to Uno
    cell6.print(outgoingMessage);
    
    targetRpm = 0;
    targetTmp = 0;
  }


if(positionIndexToUpdate == 7){
    positionIndexToUpdate = -1;

  // Continuously stream data via serial
    char targetTempStr[10];
    dtostrf(targetTmp, 5, 2, targetTempStr);      // Convert double to string

    snprintf(outgoingMessage, MESSAGE_LENGTH, "%s#%d\r\n", targetTempStr, targetRpm);
    // Send data to Uno
    cell7.print(outgoingMessage);
    
    targetRpm = 0;
    targetTmp = 0;
  }


if(positionIndexToUpdate == 8){
    positionIndexToUpdate = -1;

  // Continuously stream data via serial
    char targetTempStr[10];
    dtostrf(targetTmp, 5, 2, targetTempStr);      // Convert double to string

    snprintf(outgoingMessage, MESSAGE_LENGTH, "%s#%d\r\n", targetTempStr, targetRpm);
    // Send data to Uno
    cell8.print(outgoingMessage);
    
    targetRpm = 0;
    targetTmp = 0;
  }


if(positionIndexToUpdate == 9){
    positionIndexToUpdate = -1;

  // Continuously stream data via serial
    char targetTempStr[10];
    dtostrf(targetTmp, 5, 2, targetTempStr);      // Convert double to string

    snprintf(outgoingMessage, MESSAGE_LENGTH, "%s#%d\r\n", targetTempStr, targetRpm);
    // Send data to Uno
    cell9.print(outgoingMessage);
    
    targetRpm = 0;
    targetTmp = 0;
  }


if(positionIndexToUpdate == 10){
    positionIndexToUpdate = -1;

  // Continuously stream data via serial
    char targetTempStr[10];
    dtostrf(targetTmp, 5, 2, targetTempStr);      // Convert double to string

    snprintf(outgoingMessage, MESSAGE_LENGTH, "%s#%d\r\n", targetTempStr, targetRpm);
    // Send data to Uno
    cell10.print(outgoingMessage);
    
    targetRpm = 0;
    targetTmp = 0;
  }

  // Add some delay to avoid overwhelming the SoftwareSerial port
  delay(10);
}
