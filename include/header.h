// Description: Header file for the Torch Timer project
// Librady Includes and Definitions for Wi-Fi, NTP, OLED, LED Strip, and Web Server
#include <Arduino.h>            // Arduino Framework
#include <U8g2lib.h>            // Control on-chip OLED
#include <Wire.h>               // I2C
#define FASTLED_INTERNAL //remove annoying pragma messages
#include <FastLED.h>            // Addressable LED strip control
#include <FS.h>                 // File System
#include <NTPClient.h>          // Time
#include <WiFiUdp.h>  // WIFI UDP
#include "ledgfx.h" // LED Drawing Routines
#include "fire.h"   // LED Flame Effect
#include <WiFi.h>              // Wi-Fi
#include <AsyncTCP.h>        // Asynchronous TCP
#include <ESPAsyncWebServer.h> // Asynchronous Web Server

//NTP Configuration
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -18000, 60000); // Time Client (customeize to your settings)

// Description: This file contains the definitions for the LED strip and OLED display
#define NUM_LEDS    144    // FastLED definitions
#define LED_PIN     5      // ARGB Strip Data Pin
#define LED_BUILTIN  2     // Onboard LED pin
#define OLED_CLOCK  22     // OLED display Clock
#define OLED_DATA   21     // OLED display Data
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, OLED_CLOCK, OLED_DATA, U8X8_PIN_NONE); // Setup On Board OLED Display

// Global Variables
int g_Brightness = 128;       // 0-255 LED brightness scale
int g_PowerLimit = 6000;      // (3000) 900mW Power Limit
CRGB g_LEDs[NUM_LEDS] = {0};  // Frame buffer for FastLED
