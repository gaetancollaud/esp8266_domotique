#ifndef _MQTT_H
#define _MQTT_H

#include "Arduino.h"
#include <PubSubClient.h>
#include <map>
#include <functional>

#define MQTT_CALLBACK std::function<void(std::string, std::string)>

class MQTT {
public:
  MQTT(PubSubClient* client, const char* server, const char* deviceName);

	void init();

  void loop();

  void registerInput(std::string inputName, MQTT_CALLBACK callback);

  void writeStatus(const char* outputName, char* value);

protected:
  PubSubClient* client;
  const char* server;
  const char* deviceName;

  std::map<std::string, MQTT_CALLBACK> callbacks;

  void callback(char* topic, byte* payload, unsigned int length);
  void reconnect();

};

#endif
