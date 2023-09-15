/*
   Simple example for how to use multiple SinricPro Switch device:
   - setup 4 switch devices
   - handle request using multiple callbacks

   If you encounter any issues:
   - check the readme.md at https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md
   - ensure all dependent libraries are installed
     - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#arduinoide
     - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#dependencies
   - open serial monitor and check whats happening
   - check full user documentation at https://sinricpro.github.io/esp8266-esp32-sdk
   - visit https://github.com/sinricpro/esp8266-esp32-sdk/issues and check for existing issues or open a new one
*/

#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
#define DEBUG_ESP_PORT Serial
#define NODEBUG_WEBSOCKETS
#define NDEBUG
#endif

#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif

#include "SinricPro.h"
#include "SinricProSwitch.h"

//String WIFI_SSID  = "pcpuma";
//String WIFI_PASS = "mango2022";

#define WIFI_SSID   "IncubotCISCO"
#define WIFI_PASS  "MSP430G2"

//#define WIFI_SSID         "Totalplay-3AAA"
//#define WIFI_PASS         "3AAAFF454wKvNXwP"

#define APP_KEY           "ad1b6ffd-77b6-4586-8c1a-63afd1b64104"


#define APP_SECRET        "4e96815f-b737-45b4-8190-be9eefaa35e6-9ce38f29-c8ba-4134-8203-872ea013bcd1"

#define SWITCH_ID_1       "646ac915743f912070201d8e"
#define RELAYPIN_1        14

#define SWITCH_ID_2       "646acb6d929949c1da6e1688"
#define RELAYPIN_2        27

#define SWITCH_ID_3       "646ace4c929949c1da6e187b"
#define RELAYPIN_3        26

#define BAUD_RATE         115200                // Change baudrate to your need

bool onPowerState1(const String &deviceId, bool &state) {
  Serial.printf("Device 1 turned %s", state ? "on" : "off");
  digitalWrite(RELAYPIN_1, state ? HIGH : LOW);
  return true; // request handled properly
}

bool onPowerState2(const String &deviceId, bool &state) {
  Serial.printf("Device 2 turned %s", state ? "on" : "off");
  digitalWrite(RELAYPIN_2, state ? HIGH : LOW);
  return true; // request handled properly
}

bool onPowerState3(const String &deviceId, bool &state) {
  Serial.printf("Device 3 turned %s", state ? "on" : "off");
  digitalWrite(RELAYPIN_3, state ? HIGH : LOW);
  return true; // request handled properly
}

// setup function for WiFi connection
void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }

  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}

// setup function for SinricPro
void setupSinricPro() {
  // add devices and callbacks to SinricPro
  pinMode(RELAYPIN_1, OUTPUT);
  pinMode(RELAYPIN_2, OUTPUT);
  pinMode(RELAYPIN_3, OUTPUT);
  
  SinricProSwitch& mySwitch1 = SinricPro[SWITCH_ID_1];
  mySwitch1.onPowerState(onPowerState1);

  SinricProSwitch& mySwitch2 = SinricPro[SWITCH_ID_2];
  mySwitch2.onPowerState(onPowerState2);

    SinricProSwitch& mySwitch3 = SinricPro[SWITCH_ID_3];
  mySwitch3.onPowerState(onPowerState3);


  // setup SinricPro
  SinricPro.onConnected([]() {
    Serial.printf("Connected to SinricPro\r\n");
  });
  SinricPro.onDisconnected([]() {
    Serial.printf("Disconnected from SinricPro\r\n");
  });
  SinricPro.restoreDeviceStates(true); // Uncomment to restore the last known state from the server.

  SinricPro.begin(APP_KEY, APP_SECRET);
}

// main setup function
void setup() {
  Serial.begin(BAUD_RATE); Serial.printf("\r\n\r\n");
  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();
}
