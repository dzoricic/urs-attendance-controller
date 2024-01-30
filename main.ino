#include <WiFi.h>
#include <HTTPClient.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200); 

  WiFi.begin("testingNetwork", "test12345");
  Serial.println(F("Connecting"));

  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(F("Connected to WiFi."));
  SerialBT.begin("ESP32testWithWiFi");
  Serial.println(F("BlueTooth now available."));
}

void loop() {
  if (SerialBT.available() && WiFi.status() == WL_CONNECTED) {
    String studentUuid = SerialBT.readString();
    studentUuid.trim();

    HTTPClient http;
    String apiRequestPath = "http://192.168.225.97:8080/api/attendance/" + studentUuid;

    http.begin(apiRequestPath.c_str());
    
    http.GET();
    delay(2000);
    String payload = http.getString();
    Serial.println(payload);

    http.end();
  }
  delay(100);
}