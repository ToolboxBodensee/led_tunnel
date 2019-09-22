#include "arduino_ota_update.h"

#define NUM_RAINBOW_CYCLES 5
// animation delay config
#define ANIMATION_DELAY 10

uint32_t colorLookupRainbow(int pixel_pos)
{
    static int divisions = 7;
    static int pixel_per_div = NUMPIXELS/divisions;
    int clean_pixel_pos = pixel_pos % NUMPIXELS;
    int relative_pixel_pos = clean_pixel_pos % pixel_per_div;

    switch (clean_pixel_pos / pixel_per_div)
    {
        case 0:
        {
            //red to orange
            return pixels.Color(255,127/pixel_per_div*relative_pixel_pos,0);
        }
        case 1:
        {
            //orange to yellow
            return pixels.Color(255,127+127/pixel_per_div*relative_pixel_pos,0);
        }
        case 2:
        {
            //yellow to green
            return pixels.Color(255-255/pixel_per_div*relative_pixel_pos,255,0);
        }
        case 3:
        {
            //green to blue
            return pixels.Color(0, 255-255/pixel_per_div*relative_pixel_pos,255/pixel_per_div*relative_pixel_pos);
        }
        case 4:
        {
            //blue to dark blue
            return pixels.Color(75/pixel_per_div*relative_pixel_pos, 0, 255-127/pixel_per_div*relative_pixel_pos);
        }
        case 5:
        {
            //dark blue to violet
            return pixels.Color(75+75/pixel_per_div*relative_pixel_pos, 0, 127+127/pixel_per_div*relative_pixel_pos);
        }
        case 6:
        {
            //violet to red
            return pixels.Color(145+110/pixel_per_div*relative_pixel_pos, 0, 255-255/pixel_per_div*relative_pixel_pos);
        }
        default:
        {
            //red
            return pixels.Color(255, 0, 0);
        }
    }
}

void doRainbow()
{
    for (int offset=0;offset<=NUMPIXELS;++offset)
    {
        for(int i=0;i<=NUMPIXELS;++i)
        {
            uint32_t color = colorLookupRainbow(i+offset);
            pixels.setPixelColor(i, color);
        }

        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(ANIMATION_DELAY); // Delay for a period of time (in milliseconds).
    }
}

void led_animation_rainbow() {
    for (int i=0;i<=NUM_RAINBOW_CYCLES;++i)
    {
        doRainbow();
        ArduinoOTA.handle();
    }
}
