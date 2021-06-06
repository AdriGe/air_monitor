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

class LedStrip {
    public:
        LedStrip();
        void set_color(CRGB color);
        void set_color(uint8_t red, uint8_t green, uint8_t blue);
        void set_brightness(uint8_t brightness);
        void set_color_from_aqi(int aqi);
        void set_effect_breathing_delay(int delay);
        void set_effect_breath();
        bool set_spinning_effect(CHSV hsv, uint8_t min_brightness, uint8_t max_brightness, unsigned long animation_duration);
        void set_spinning_array(CHSV hsv, uint8_t min_brightness, uint8_t max_brightness);

    private:
        CRGB m_leds[NB_LEDS];
        unsigned long m_effect_breath_last_exec_time;
        unsigned long m_effect_breath_iteration;
        int m_effect_breath_delay;
        bool m_is_animation_running;
        unsigned long m_animation_start_time;
        uint8_t m_spinning_seed[NB_LEDS];
        uint8_t m_spinning_action[NB_LEDS];
        uint8_t m_spinning_min_max_index[NB_LEDS];

        uint16_t animation_spinning_substract_from_seed(uint16_t current_pixel, uint8_t current_index);
        uint16_t animation_spinning_add_from_seed(uint16_t current_pixel, uint8_t current_index);
        uint16_t animation_spinning_substract(uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max);
        uint16_t animation_spinning_add(uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max);
        void animation_spinning_set_min_max_index(uint16_t current_pixel, uint8_t current_index);
        void animation_spinning_set_action(uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max);
        void animation_spinning_set_pixel_color(uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max);
};

#endif