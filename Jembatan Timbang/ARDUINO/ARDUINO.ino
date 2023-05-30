#include <SoftwareSerial.h>

SoftwareSerial mySerial(6, 7); // RX, TX

String inString, dataStr[2];
bool comComp = false;
int dataSave = 0, dataToSendF, dataToSendS;
float countData;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mySerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //  dummyData();
  mySerialRead();
  if (comComp == true) {
    for (int i = 0; i < inString.length(); i++) {
      if (inString[i] == 'w' || inString[i] == 'n') {
        dataStr[0] = "";
        dataStr[1] = "";
        dataSave = 0;
      }
      else if (inString[i] == 'k') {
        break;
      }
      else if (inString[i] == '.') {
        dataSave = 1;
      }
      else {
        dataStr[dataSave] += inString[i];
      }
    }
    dataToSendF = dataStr[0].toInt();
    dataToSendS = dataStr[1].toInt();
    Serial.print(dataToSendF);
    Serial.print(".");
    Serial.println(dataToSendS);
    inString = "";
    comComp = false;
  }
  delay(1000);
}

void mySerialRead() {
  while (mySerial.available()) {
    char inChar = (char)mySerial.read();
    inString += inChar;
    if (inChar == '\n') {
      comComp = true;
    }
  }
}

//dummy
//void dummyData() {
//  Serial.println(countData);
//  countData += 10.2;
//  delay(1000);
//}
