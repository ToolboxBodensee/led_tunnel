#include <ESP8266WiFi.h>

#include "ota_update.h"

//This is running on a nodeMCU V1.0 ESP12E
 
#define FirmwareVer  7 // Bitte hier Version der Firmware eintragen
#define URL_Mit_Version_Info "http://missing/repo/esp/led_tunnel/httpUpdate.txt" 
#define URL_Mit_Firmware "http://missing/repo/esp/led_tunnel/httpUpdate.bin"
HTTPClient http;
 
const char* ssid = "<ssid>";
const char* password = "<pw>";

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to GPIO6 / D1
#define PIN            5

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      150

#define LARSON_WIDTH 5

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 20; // delay for half a second
 
void setup()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Start");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("O");
  }                                     // Verbunden
  Serial.println("");
  Serial.println("Verbunden");

  OTA_CONFIG ota_config = {
    .version = FirmwareVer,
    .check_url = URL_Mit_Version_Info,
    .binary_url = URL_Mit_Firmware,
    .debug = true,
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
