#ifndef DEF_LED_STRIP
#define DEF_LED_STRIP

#include <Arduino.h>
#include <FastLED.h>
#include <math.h>

const int DATA_PIN = 3;
const int NB_LEDS = 10;
const uint8_t ANIMATION_SPINNING_STATE_SUBSTRACT_FROM_SEED = 0;
const uint8_t ANIMATION_SPINNING_STATE_SUBSTRACT = 1;
const uint8_t ANIMATION_SPINNING_STATE_ADD_FROM_SEED = 2;
const uint8_t ANIMATION_SPINNING_STATE_ADD = 3;
const uint8_t CURRENT_ANIMATION_NONE = 0;
const uint8_t CURRENT_ANIMATION_SPINNING = 1;
const uint8_t CURRENT_ANIMATION_BREATHING = 2;

class LedStrip {
    public:
        LedStrip();
        void set_color(CRGB color);
        void set_color(uint8_t red, uint8_t green, uint8_t blue);
        void set_brightness(uint8_t brightness);
        void set_color_from_aqi(int aqi);
        bool set_breathing_animation(CHSV hsv, uint8_t brightness_min, uint8_t brightness_max, unsigned long animation_duration, unsigned long animation_duration_low_static);
        bool set_spinning_animation(CHSV hsv, uint8_t brightness_min, uint8_t brightness_max, unsigned long animation_duration);

    private:
        CRGB m_leds[NB_LEDS];
        bool m_is_animation_running;
        uint8_t m_current_animation;
        unsigned long m_animation_start_time;

        uint8_t m_spinning_seed[NB_LEDS];
        uint8_t m_spinning_action[NB_LEDS];
        uint8_t m_spinning_min_max_index[NB_LEDS];

        void animation_spinning_set_array(CHSV hsv, uint8_t min_brightness, uint8_t max_brightness);
        uint16_t animation_spinning_substract_from_seed(uint16_t current_pixel, uint8_t current_index);
        uint16_t animation_spinning_add_from_seed(uint16_t current_pixel, uint8_t current_index);
        uint16_t animation_spinning_substract(uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max);
        uint16_t animation_spinning_add(uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max);
        void animation_spinning_set_min_max_index(uint16_t current_pixel, uint8_t current_index);
        void animation_spinning_set_action(uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max);
        void animation_spinning_set_pixel_color(CHSV hsv, uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max);
};

#endif