#ifndef _OTA_H
#define _OTA_H

#include <ArduinoOTA.h>
#include "Arduino.h"

/**
 * Abstract class to help debug class
 */
class OTA {
public:
  OTA(const char* deviceName, int statudLed);

	void init();

  void loop();

protected:
  const char* deviceName;
  int statusLed;
  bool wasOn;
};

#endif
