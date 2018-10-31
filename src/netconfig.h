#ifndef __NETCONFIG_H__
#define __NETCONFIG_H__

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <airkiss.h>

#define LED_PIN 2
#define DEFAULT_LAN_PORT  12476

void initNetConfig();
bool checkConnect();
bool startAirkiss();
bool deleteConfig();

void startDiscover();

#endif
