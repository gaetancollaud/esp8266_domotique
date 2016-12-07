# esp8266_domotique

## Configuration
Create a file called config/wifi.h
```c
#ifndef _WIFI_H
#define _WIFI_H

const char* CONFIG_SSID = "your ssd here";
const char* CONFIG_PASSWORD = "your_password";
const char* CONFIG_MQTT_SERVER = "openhab.your.domain";
const int CONFIG_MQTT_PORT = 1883;

#endif

```
