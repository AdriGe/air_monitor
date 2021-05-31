#ifndef DEF_LED_STRIP
#define DEF_LED_STRIP

#include <Arduino.h>
#include <FastLED.h>
#include <math.h>

const int DATA_PIN = 3;
const int NB_LEDS = 10;

class LedStrip {
    public:
        LedStrip();
        void set_color(CRGB color);
        void set_color(uint8_t red, uint8_t green, uint8_t blue);
        void set_brightness(uint8_t brightness);
        void set_color_from_aqi(int aqi);
        void set_effect_breathing_delay(int delay);
        void set_effect_breath();

    private:
        CRGB m_leds[10];
        unsigned long m_effect_breath_last_exec_time;
        unsigned long m_effect_breath_iteration;
        int m_effect_breath_delay;
        
};

#endif