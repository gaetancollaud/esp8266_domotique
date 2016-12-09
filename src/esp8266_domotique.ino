#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

#include <OTA.h>
#include <ws2812.h>
#include <mqtt.h>

#include "../config/wifi.h"

// #define BUILTIN_LED 2
#define BUILTIN_LED 2
#define PIN_RELAY 5
#define NB_PIXELS 12

// Update these with values suitable for your network.

const char* deviceName = "circleled";

WiFiClient espClient;
PubSubClient client(espClient);

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(NB_PIXELS, 2);
NeoPixelAnimator animations(NB_PIXELS, NEO_MILLISECONDS);

OTA ota(deviceName, BUILTIN_LED);
WS2812 ledStip(&strip, &animations);
MQTT mqtt(&client, CONFIG_MQTT_SERVER, deviceName);

long lastMsg = 0;
char msg[50];
int value = 0;


void testCallback(std::string input, std::string value){
  Serial.print("callback\t");
  Serial.print(input.c_str());
  Serial.print("\t");
  Serial.println(value.c_str());
  ledStip.setColor(value);
}

void setup() {
  Serial.begin(115200);

  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
  digitalWrite(PIN_RELAY, LOW);


  setup_wifi();

  mqtt.init();
  ota.init();
  ledStip.init();

  mqtt.registerInput("TestColor", testCallback);
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

void loop() {
  mqtt.loop();
  ota.loop();
  ledStip.loop();
}
