#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>

#include <addons/TokenHelper.h>

#include <addons/RTDBHelper.h>

#define WIFI_SSID "Redmi8"
#define WIFI_PASSWORD "rast1234"

#define API_KEY "AIzaSyBYGToLczRNnwuhVis5oYKTXqIui06-8Co"

#define DATABASE_URL "https://indicator-5bcde-default-rtdb.asia-southeast1.firebasedatabase.app/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

#define USER_EMAIL "ptsks.server@gmail.com"
#define USER_PASSWORD "sks2023!"

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
WiFiMulti multi;
#endif

String inString, dataStr;
bool comComp = false;
int dataSave = 0;
float dataToSendF;

void setup()
{

  Serial.begin(115200);
  Serial2.begin(115200);

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  multi.addAP(WIFI_SSID, WIFI_PASSWORD);
  multi.run();
#else
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
#endif

  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    if (millis() - ms > 10000)
      break;
#endif
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  config.api_key = API_KEY;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  config.database_url = DATABASE_URL;

  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

#if defined(ESP8266)
  fbdo.setBSSLBufferSize(2048 /* Rx buffer size in bytes from 512 - 16384 */, 2048 /* Tx buffer size in bytes from 512 - 16384 */);
#endif
  fbdo.setResponseSize(2048);

  Firebase.begin(&config, &auth);

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  config.wifi.clearAP();
  config.wifi.addAP(WIFI_SSID, WIFI_PASSWORD);
#endif

  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);

  config.timeout.serverResponse = 10 * 1000;

}

void loop()
{
  SerialRead();
  // Firebase.ready() should be called repeatedly to handle authentication tasks.

  if (comComp == true) {
    for (int i = 0; i < inString.length(); i++) {
      dataStr += inString[i];
    }
    Serial.print("inString : ");
    Serial.print(inString);
    dataToSendF = dataStr.toFloat();
    Serial.print("dataToSend : ");
    Serial.println(dataToSendF);
    inString = "";
    dataStr = "";
    comComp = false;
  }

  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();

    Serial.printf("Set float... %s\n", Firebase.RTDB.setFloat(&fbdo, F("/IndicatorValue"), dataToSendF) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get float... %s\n", Firebase.RTDB.getFloat(&fbdo, F("/IndicatorValue")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

    Serial.println();
  }
}

void SerialRead() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inString += inChar;
    if (inChar == '\n') {
      comComp = true;
    }
  }
}
