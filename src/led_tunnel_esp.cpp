#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include <ArduinoOTA.h>

#include "ota_update.h"
#include "eeprom_settings.h"

#define URL_Version_Info_Default "http://releases.tbbs.me/version"
#define URL_Firmware_Default "http://releases.tbbs.me/d1_mini.bin"

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

void onFirmwareUpdateDone(unsigned int newVersion)
{
    //update was done, save new version number
    cfg.firmwareVer = newVersion;
    saveConfig(cfg);
}

void setup_arduino_ota()
{
    ArduinoOTA.setHostname("LedTunnel");
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else { // U_SPIFFS
            type = "filesystem";
        }

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
            Serial.println("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
            Serial.println("End Failed");
        }
    });
    ArduinoOTA.begin();
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
        cfg.firmwareVer = 0;
        strncpy(cfg.version_info_url, URL_Version_Info_Default, sizeof(URL_Version_Info_Default));
        strncpy(cfg.version_update_url, URL_Firmware_Default, sizeof(URL_Firmware_Default));
        cfg.debug = 1;
        saveConfig(cfg);
    }

    Serial.begin(115200);
    Serial.println("");
    Serial.println("Start");

    WiFi.hostname("LedTunnel");
    WiFi.mode(WIFI_STA);
    WiFi.begin(cfg.SSID, cfg.password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print("O");
    }                                     // Verbunden
    Serial.println("");
    Serial.println("Verbunden");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    setup_arduino_ota();

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
    ArduinoOTA.handle();
}
