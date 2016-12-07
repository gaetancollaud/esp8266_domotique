
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include "../config/wifi.h"

// #define BUILTIN_LED 2
#define BUILTIN_LED 2
#define PIN_RELAY 5

// Update these with values suitable for your network.

const char* deviceName = "sapin";
const char* topicIn = "/openhab/out/Salon2Relay1/command";
const char* topicOut = "/openhab/out/Salon2Relay1/state";


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  Serial.begin(115200);

  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
  digitalWrite(PIN_RELAY, LOW);

  setup_wifi();
  client.setServer(CONFIG_MQTT_SERVER, CONFIG_MQTT_PORT);
  client.setCallback(callback);

  setupOTA();

}

  bool wasOn = true;
void setupOTA(){
  ArduinoOTA.setHostname(deviceName);
  // ArduinoOTA.setPassword("admin");

  ArduinoOTA.onStart([]() {
      Serial.println("Start updating ");
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
      digitalWrite(BUILTIN_LED, HIGH);
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      wasOn = !wasOn;
      digitalWrite(BUILTIN_LED, wasOn ? LOW : HIGH);
    });
    ArduinoOTA.onError([](ota_error_t error) {
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

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(CONFIG_SSID);

  WiFi.begin(CONFIG_SSID, CONFIG_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == 'O' && (char)payload[1]=='N') {
    digitalWrite(PIN_RELAY, HIGH);
  } else {
    digitalWrite(PIN_RELAY, LOW);
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(deviceName)) {
      digitalWrite(BUILTIN_LED, LOW);
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(topicOut, "OFF");
      // ... and resubscribe
      client.subscribe(topicIn);
    } else {
      digitalWrite(BUILTIN_LED, HIGH);
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  ArduinoOTA.handle();
}
