#include <SoftwareSerial.h>

SoftwareSerial mySerial(6, 7); // RX, TX

String inString, dataStr, dataPharse;
String decimal;
bool comComp = false;
int dataToSend, dec;
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
      if (inString[i] == 0x02 || inString[i] == '+' || inString[i] == '-') {
        dataStr = "";
      }
      else if (inString[i] == 0x03) {
        break;
      }
      else {
        dataStr += inString[i];
      }
    }
    for (int i = 0; i < 6; i++) {
      dataPharse += dataStr[i];
    }
    decimal = dataStr[6];
    dataToSend = dataPharse.toInt();
    dec = decimal.toInt();
    Serial.println(dataToSend);
    Serial.println(dataToSend / pembagi(dec));
    inString = "";
    dataPharse = "";
    comComp = false;
  }
  delay(1000);
}

void mySerialRead() {
  while (mySerial.available()) {
    char inChar = (char)mySerial.read();
    inString += inChar;
    if (inChar == 0x03) {
      comComp = true;
    }
  }
}

int pembagi(int value) {
  switch (value) {
    case 0 :
      return 1;
      break;
    case 1 :
      return 10;
      break;
    case 2 :
      return 100;
      break;
    case 3 :
      return 1000;
      break;
    case 4 :
      return 10000;
      break;
  }
}

//dummy
//void dummyData() {
//  Serial.println(countData);
//  countData += 10.2;
//  delay(1000);
//}
