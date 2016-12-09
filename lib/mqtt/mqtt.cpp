#include "mqtt.h"

const char* topicIn = "/openhab/out/TestColor/command";
const char* topicOut = "/openhab/out/TestColor/state";

MQTT::MQTT(PubSubClient* client, const char* server, const char* deviceName) : client(client), server(server), deviceName(deviceName){
}

void MQTT::init(){
  client->setServer(this->server, 1883);
  client->setCallback([&](char* topic, byte* payload, unsigned int length) {
    this->callback(topic, payload, length);
  });
}

void MQTT::loop(){

    if (!client->connected()) {
      reconnect();
    }
    client->loop();


}

void MQTT::callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  std::string strTopic(topic);
  strTopic = strTopic.substr(13, strTopic.length()-strTopic.find("/command")+1);
  if(this->callbacks.find(strTopic) != this->callbacks.end()){
      this->callbacks[strTopic](strTopic, std::string((char*)payload, length));
  }else{
    Serial.print("no callback for ");
    Serial.println(strTopic.c_str());
  }
}

void MQTT::reconnect() {
  // Loop until we're reconnected
  while (!client->connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client->connect(deviceName)) {
      digitalWrite(BUILTIN_LED, LOW);
      Serial.println("connected");
      // Once connected, publish an announcement...
      client->publish(topicOut, "OFF");
      // ... and resubscribe
      client->subscribe(topicIn);
    } else {
      digitalWrite(BUILTIN_LED, HIGH);
      Serial.print("failed, rc=");
      Serial.print(client->state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void MQTT::registerInput(std::string inputName, MQTT_CALLBACK callback){
  Serial.print("Register new callback : ");
  Serial.println(inputName.c_str());
  this->callbacks[inputName] = callback;
}
