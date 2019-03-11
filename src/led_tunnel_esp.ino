#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <EEPROM.h>
#include <WString.h>

typedef struct {
    int initialized;                        // 0=no configuration, 1=valid configuration
    char SSID[31];                    // SSID of WiFi
    char password[31];                // Password of WiFi
    int firmwareVer;
    char version_info_url[120];
    char version_update_url[120];
    int debug;
} eepromData_t;

#include "ota_update.h"

#define URL_Version_Info_Default "https://github.com/ToolboxBodensee/led_tunnel/tree/master/bin/httpUpdate.txt"
#define URL_Firmware_Default "https://github.com/ToolboxBodensee/led_tunnel/tree/master/bin/httpUpdate.bin"

//This is running on a nodeMCU V1.0 ESP12E

eepromData_t cfg;
int cfgStart = 0;

HTTPClient http;

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to GPIO6 / D1
#define NODEMCU
#ifdef NODEMCU
#define PIN D5
#else
#define PIN 5
#endif

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      150

#define LARSON_WIDTH 5

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 20; // delay config

void eraseConfig() {
    // Reset EEPROM bytes to '0' for the length of the data structure
    EEPROM.begin(4095);
    for (unsigned int i = cfgStart ; i < sizeof(cfg) ; i++) {
        EEPROM.write(i, 0);
    }
    delay(200);
    EEPROM.commit();
    EEPROM.end();
}

void saveConfig() {
    // Save configuration from RAM into EEPROM
    EEPROM.begin(4095);
    EEPROM.put( cfgStart, cfg );
    delay(200);
    EEPROM.commit();                      // Only needed for ESP8266 to get data written
    EEPROM.end();                         // Free RAM copy of structure
}

void loadConfig() {
    // Loads configuration from EEPROM into RAM
    EEPROM.begin(4095);
    EEPROM.get( cfgStart, cfg );
    EEPROM.end();
}

void setup()
{
    loadConfig();
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
        saveConfig();
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
        .debug = cfg.debug,
    };

    FirmwareUpdate(ota_config);

    //led stuff
    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
    #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
    #endif
    // End of trinket special code

    pixels.begin(); // This initializes the NeoPixel library.
}

void doLedAnimation(uint32_t color1, uint32_t color2)
{
    for(int i=0;i<NUMPIXELS-LARSON_WIDTH;++i)
    {

        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        if (i!=0)
        pixels.setPixelColor(i-1, color1); // set pixel to color1.
        for (int w=0;w<LARSON_WIDTH;++w){
            pixels.setPixelColor(i+w, color2); // set pixel to color2.
        }

        pixels.show(); // This sends the updated pixel color to the hardware.

        delay(delayval); // Delay for a period of time (in milliseconds).

    }
    for(int i=NUMPIXELS-LARSON_WIDTH;i>=0;--i)
    {
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        for (int w=0;w<LARSON_WIDTH;++w){
            pixels.setPixelColor(i+w, color2); // set pixel to color2.
        }
        if (i!=0)
        pixels.setPixelColor(i+LARSON_WIDTH, color1); // set pixel to color1.

        pixels.show(); // This sends the updated pixel color to the hardware.

        delay(delayval); // Delay for a period of time (in milliseconds).

    }
}

void loop()
{
    doLedAnimation(pixels.Color(20,50,20), pixels.Color(150,0,0));
    doLedAnimation(pixels.Color(0,50,50), pixels.Color(0,150,0));
    doLedAnimation(pixels.Color(50,0,50), pixels.Color(0,0,150));
}
