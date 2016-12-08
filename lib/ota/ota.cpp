#include "ota.h"

OTA::OTA(const char* deviceName, int statusLed):deviceName(deviceName), statusLed(statusLed), wasOn(false){
}

void OTA::init(){

  pinMode(statusLed, OUTPUT);

  ArduinoOTA.setHostname(deviceName);
  // ArduinoOTA.setPassword("admin");

  ArduinoOTA.onStart([&]() {
    Serial.println("Start updating ");
  });
  ArduinoOTA.onEnd([&]() {
    Serial.println("\nEnd");
    digitalWrite(statusLed, HIGH);
  });
  ArduinoOTA.onProgress([&](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    wasOn = !wasOn;
    digitalWrite(statusLed, wasOn ? LOW : HIGH);
  });
  ArduinoOTA.onError([&](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready for OTA");
}

void OTA::loop(){
  ArduinoOTA.handle();
}
