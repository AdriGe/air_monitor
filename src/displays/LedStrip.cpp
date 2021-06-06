#include <displays/LedStrip.h>

using namespace std;

LedStrip::LedStrip()
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(m_leds, NB_LEDS);
    m_is_animation_running = false;
    m_current_animation = CURRENT_ANIMATION_NONE;
}


void LedStrip::set_brightness(uint8_t brightness) {
    FastLED.setBrightness(brightness);
}

void LedStrip::set_color_from_aqi(int aqi) {
    Serial.print("[LedStrip] Change color (AQI = ");
    Serial.print(aqi);
    Serial.println(")");
    
    if      (aqi == 1)  { set_color(0,255,0); }
    else if (aqi == 2)  { set_color(191,255,0); }
    else if (aqi == 3)  { set_color(255,255,0); }
    else if (aqi == 4)  { set_color(255,153,0); }
    else if (aqi == 5)  { set_color(255,0,0); }
    else                { set_color(255,255,255); }
}


bool LedStrip::set_breathing_animation(CHSV hsv, uint8_t brightness_min, uint8_t brightness_max, unsigned long animation_duration, unsigned long animation_duration_low_static){
    if (!m_is_animation_running || m_current_animation != CURRENT_ANIMATION_BREATHING) {
        Serial.println("[LEDStrip] Start breathing effect");
        m_is_animation_running = true;
        m_current_animation = CURRENT_ANIMATION_BREATHING;
        m_animation_start_time = millis();
    }

    unsigned long animation_time = millis() - m_animation_start_time;

    if(animation_time < animation_duration){
        int animation_percent = map(animation_time, 0, animation_duration, 0, 100);
        int brightness_target = (brightness_max - brightness_min) * exp( (-1.0 * sq(animation_percent - 50.0)) / (2.0 * sq(20.0))) + brightness_min;

        for(int i = 0; i<NB_LEDS; i++){
            m_leds[i].setHSV(hsv.hue, hsv.saturation, brightness_target);
        }

        FastLED.show();
    }
    
    if (millis() > m_animation_start_time + animation_duration + animation_duration_low_static) {
        m_is_animation_running = false;
        m_current_animation = CURRENT_ANIMATION_NONE;
        Serial.println("[LedStrip] End breathing animation");
        return true;
    } else {
        return false;
    }        
}


bool LedStrip::set_spinning_animation(CHSV hsv, uint8_t brightness_min, uint8_t brightness_max, unsigned long animation_duration) {

    if (!m_is_animation_running || m_current_animation != CURRENT_ANIMATION_SPINNING) {
        Serial.println("[LEDStrip] Start spinning");
        m_is_animation_running = true;
        m_current_animation = CURRENT_ANIMATION_SPINNING;
        m_animation_start_time = millis();
        animation_spinning_set_array(hsv, brightness_min, brightness_max);
    }

    int brightness_difference = brightness_max - brightness_min;
    unsigned long animation_time = millis() - m_animation_start_time;
    int animation_percent = map(animation_time, 0, animation_duration, 0, 100);
    int matching_index = map(animation_percent, 0, 100, 0, brightness_difference-1);

    for(int i = 0; i<NB_LEDS; i++){
        animation_spinning_set_action(i, matching_index, brightness_min, brightness_max);
        animation_spinning_set_pixel_color(hsv, i, matching_index, brightness_min, brightness_max);
    }

    FastLED.show();

    if (millis() > m_animation_start_time + animation_duration) {
        m_is_animation_running = false;
        m_current_animation = CURRENT_ANIMATION_NONE;
        Serial.println("[LedStrip] End spinning");
        return true;
    } else {
        return false;
    }        
}


void LedStrip::animation_spinning_set_array(CHSV hsv, uint8_t brightness_min, uint8_t brightness_max) {
    int center_index = (NB_LEDS/2) - 1;

    for(int i = 0; i<NB_LEDS; i++){
        uint8_t pixel_position_percent = map(i, 0, NB_LEDS-1, 0, 100);
        uint8_t brightness_diff = brightness_max - brightness_min;

        // This function is used to draw the spinning seed (can be linear or not)
        // f([0 ; 100]) = [0 ; 100]
        // Also : f(0) = 0 and f(100) = 0
        float brightness_diff_percent = abs(100*sin(pixel_position_percent/31.830989));

        uint8_t pixel_offset = brightness_diff * (brightness_diff_percent/100);
        uint8_t pixel_brightness = brightness_min + pixel_offset;
        
        m_spinning_seed[i] = pixel_brightness;

        if(i<=center_index) {
            m_spinning_action[i] = ANIMATION_SPINNING_STATE_SUBSTRACT_FROM_SEED;
        } else {
            m_spinning_action[i] = ANIMATION_SPINNING_STATE_ADD_FROM_SEED;
        }
    }
}


uint16_t LedStrip::animation_spinning_substract_from_seed(uint16_t current_pixel, uint8_t current_index) {
    return m_spinning_seed[current_pixel] - (2*current_index);
}


uint16_t LedStrip::animation_spinning_add_from_seed(uint16_t current_pixel, uint8_t current_index) {
    return m_spinning_seed[current_pixel] + (2*current_index);
}


uint16_t LedStrip::animation_spinning_substract(uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max) {
    return brightness_max - ( 2 * ( current_index - m_spinning_min_max_index[current_pixel] ) );
}


uint16_t LedStrip::animation_spinning_add(uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max) {
    return brightness_min + ( 2 * ( current_index - m_spinning_min_max_index[current_pixel] ) );
}


void LedStrip::animation_spinning_set_min_max_index(uint16_t current_pixel, uint8_t current_index){
    if(current_index - 1 < 0){
        m_spinning_min_max_index[current_pixel] = 0;
    } else {
        m_spinning_min_max_index[current_pixel] = current_index - 1;
    }
}


void LedStrip::animation_spinning_set_action(uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max){

    if((m_spinning_action[current_pixel] == ANIMATION_SPINNING_STATE_SUBSTRACT_FROM_SEED) &&
       (animation_spinning_substract_from_seed(current_pixel, current_index) < brightness_min)) 
    {
        m_spinning_action[current_pixel] = ANIMATION_SPINNING_STATE_ADD;
        animation_spinning_set_min_max_index(current_pixel, current_index);

    } 
    else if((m_spinning_action[current_pixel] == ANIMATION_SPINNING_STATE_ADD_FROM_SEED) &&
            (animation_spinning_add_from_seed(current_pixel, current_index) > brightness_max))
    {
        m_spinning_action[current_pixel] = ANIMATION_SPINNING_STATE_SUBSTRACT;
        animation_spinning_set_min_max_index(current_pixel, current_index);

    } 
    else if((m_spinning_action[current_pixel] == ANIMATION_SPINNING_STATE_SUBSTRACT) && 
            (animation_spinning_substract(current_pixel, current_index, brightness_min, brightness_max) < brightness_min))
    {
        m_spinning_action[current_pixel] = ANIMATION_SPINNING_STATE_ADD;
        animation_spinning_set_min_max_index(current_pixel, current_index);

    } 
    else if((m_spinning_action[current_pixel] == ANIMATION_SPINNING_STATE_ADD) && 
            (animation_spinning_add(current_pixel, current_index, brightness_min, brightness_max) > brightness_max)) 
    {
        m_spinning_action[current_pixel] = ANIMATION_SPINNING_STATE_SUBSTRACT;
        animation_spinning_set_min_max_index(current_pixel, current_index);
    }
}


void LedStrip::animation_spinning_set_pixel_color(CHSV hsv, uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max){
        int brightness_target;

        switch (m_spinning_action[current_pixel])
        {
            case ANIMATION_SPINNING_STATE_SUBSTRACT_FROM_SEED:
                brightness_target = animation_spinning_substract_from_seed(current_pixel, current_index);
                break;

            case ANIMATION_SPINNING_STATE_ADD_FROM_SEED:
                brightness_target = animation_spinning_add_from_seed(current_pixel, current_index);
                break;

            case ANIMATION_SPINNING_STATE_SUBSTRACT:
                brightness_target = animation_spinning_substract(current_pixel, current_index, brightness_min, brightness_max);
                break;

            case ANIMATION_SPINNING_STATE_ADD:
                brightness_target = animation_spinning_add(current_pixel, current_index, brightness_min, brightness_max);
                break;

            default:
                brightness_target = 0;
                break;
        }

        m_leds[current_pixel].setHSV(hsv.hue, hsv.saturation, brightness_target);
}