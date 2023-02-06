
#include <esp32-hal.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <time.h>
#include <string>
#include <cstdlib>


// Wifi network station credentials
#define WIFI_SSID "Becodiagonal_2.4G"
#define WIFI_PASSWORD ""
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "" 

#define LED_15 15
#define LED_2 2
#define LED_4 4

#define COOL_5 5
#define COOL_18 18
#define COOL_19 19


WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);