#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include "ota_update.h"
#include "eeprom_settings.h"

#define URL_Version_Info_Default "https://github.com/ToolboxBodensee/led_tunnel/releases/download/travis-latest/version"
#define URL_Firmware_Default "https://github.com/ToolboxBodensee/led_tunnel/releases/download/travis-latest/d1_mini.bin"

// Which pin on the MCU is connected to the NeoPixels?
//#define NODEMCU
#ifndef NODEMCU
#define PIN D5
#else
#define PIN 5
#endif

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 150

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#include "led_animation_simple_larson_scanner.h"

//This is running on a nodeMCU V1.0 ESP12E or wemos d1 mini

eepromData_t cfg;
HTTPClient http;

void onFirmwareUpdateDone(unsigned long newVersion)
{
    //update was done, save new version number
    cfg.firmwareVer = newVersion;
    saveConfig(cfg);
}

void setup()
{
    loadConfig(cfg);
    if (cfg.initialized != 1)
    {
        //not initialized
        cfg.initialized = 1;
        strncpy(cfg.SSID, "<ssid>", sizeof("<ssid>"));
        strncpy(cfg.password, "<password>", sizeof("<password>"));
        cfg.firmwareVer = 9;
        strncpy(cfg.version_info_url, URL_Version_Info_Default, sizeof(URL_Version_Info_Default));
        strncpy(cfg.version_update_url, URL_Firmware_Default, sizeof(URL_Firmware_Default));
        cfg.debug = 1;
        saveConfig(cfg);
    }

    Serial.begin(115200);
    Serial.println("");
    Serial.println("Start");
    WiFi.mode(WIFI_STA);
    WiFi.begin(cfg.SSID, cfg.password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print("O");
    }                                     // Verbunden
    Serial.println("");
    Serial.println("Verbunden");

    OTA_CONFIG ota_config = {
        .version = cfg.firmwareVer,
        .check_url = cfg.version_info_url,
        .binary_url = cfg.version_update_url,
        .debug = (bool) cfg.debug,
    };

    FirmwareUpdate(ota_config, &onFirmwareUpdateDone);

    //led stuff
    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
    #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
    #endif
    // End of trinket special code

    pixels.begin(); // This initializes the NeoPixel library.
}

void loop()
{
    led_animation_simple_larson_scanner();
}
