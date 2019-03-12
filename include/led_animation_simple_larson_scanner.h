//scanner width
#define LARSON_WIDTH 5
// animation delay config
#define ANIMATION_DELAY 20

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

        delay(ANIMATION_DELAY); // Delay for a period of time (in milliseconds).

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

        delay(ANIMATION_DELAY); // Delay for a period of time (in milliseconds).

    }
}

void led_animation_simple_larson_scanner() {
    doLedAnimation(pixels.Color(20,50,20), pixels.Color(150,0,0));
    doLedAnimation(pixels.Color(0,50,50), pixels.Color(0,150,0));
    doLedAnimation(pixels.Color(50,0,50), pixels.Color(0,0,150));
}
