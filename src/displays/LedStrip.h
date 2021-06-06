#ifndef DEF_LED_STRIP
#define DEF_LED_STRIP

#include <Arduino.h>
#include <FastLED.h>
#include <math.h>
#include <config/config.h>

#define ANIMATION_SPIN_STATE_SUBSTRACT_FROM_SEED 0
#define ANIMATION_SPIN_STATE_SUBSTRACT 1
#define ANIMATION_SPIN_STATE_ADD_FROM_SEED 2
#define ANIMATION_SPIN_STATE_ADD 3
#define CURRENT_ANIMATION_NONE 0
#define CURRENT_ANIMATION_SPINNING 1
#define CURRENT_ANIMATION_BREATHING 2

class LedStrip {
    public:
        LedStrip();
        void set_color(CRGB color);
        void set_color(uint8_t red, uint8_t green, uint8_t blue);
        void set_brightness(uint8_t brightness);
        void set_color_from_aqi(int aqi);
        bool set_animation_breath(CHSV hsv, uint8_t brightness_min, uint8_t brightness_max, unsigned long animation_duration, unsigned long animation_duration_low_static);
        bool set_animation_spin(CHSV hsv, uint8_t brightness_min, uint8_t brightness_max, unsigned long animation_duration);

    private:
        CRGB m_leds[NB_LEDS];
        bool m_is_animation_running;
        uint8_t m_current_animation;
        unsigned long m_animation_start_time;

        uint8_t m_spin_seed[NB_LEDS];
        uint8_t m_spin_action[NB_LEDS];
        uint8_t m_spin_min_max_index[NB_LEDS];

        void animation_spin_set_array(CHSV hsv, uint8_t min_brightness, uint8_t max_brightness);
        uint16_t animation_spin_substract_from_seed(uint16_t current_pixel, uint8_t current_index);
        uint16_t animation_spin_add_from_seed(uint16_t current_pixel, uint8_t current_index);
        uint16_t animation_spin_substract(uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max);
        uint16_t animation_spin_add(uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max);
        void animation_spin_set_min_max_index(uint16_t current_pixel, uint8_t current_index);
        void animation_spin_set_action(uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max);
        void animation_spin_set_pixel_color(CHSV hsv, uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max);
};

#endif