#include <displays/LedStrip.h>

using namespace std;

LedStrip::LedStrip()
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(m_leds, NB_LEDS);
    m_effect_breath_last_exec_time = millis();
    m_effect_breath_iteration = 0;
    m_effect_breath_delay = 30;

    m_is_animation_running = false;
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

void LedStrip::set_effect_breath()
{
    // https://arduinoelectronics.wordpress.com/2015/05/12/non-blocking-breathing-led/
    
    if( (m_effect_breath_last_exec_time + m_effect_breath_delay) < millis() ){
        m_effect_breath_last_exec_time = millis();
        
        float val = (exp(sin(m_effect_breath_iteration/2000.0*PI*10)) - 0.36787944)*108.0; // this is the math function recreating the effect
        
        //analogWrite(ledPin, val);  // PWM
        FastLED.setBrightness(val);
        FastLED.show();

        m_effect_breath_iteration = m_effect_breath_iteration + 1;
    }
}

void LedStrip::set_effect_breathing_delay (int delay) { m_effect_breath_delay = delay; }


bool LedStrip::set_spinning_effect(CHSV hsv, uint8_t brightness_min, uint8_t brightness_max, unsigned long animation_duration) {

    if (!m_is_animation_running) {
        Serial.println("[LEDStrip] Start spinning");
        m_is_animation_running = true;
        m_animation_start_time = millis();
        set_spinning_array(hsv, brightness_min, brightness_max);
    }

    int brightness_difference = brightness_max - brightness_min;
    unsigned long animation_time = millis() - m_animation_start_time;
    int animation_percent = map(animation_time, 0, animation_duration, 0, 100);
    int matching_index = map(animation_percent, 0, 100, 0, brightness_difference-1);

    for(int i = 0; i<NB_LEDS; i++){

        if(m_spinning_action[i] == ANIMATION_SPINNING_STATE_SUBSTRACT_FROM_SEED) {

            if(animation_spinning_substract_from_seed(i, matching_index) < brightness_min){
                m_spinning_action[i] = ANIMATION_SPINNING_STATE_ADD;
                animation_spinning_set_min_max_index(i, matching_index);
            }

        } else if(m_spinning_action[i] == ANIMATION_SPINNING_STATE_ADD_FROM_SEED){

            if(animation_spinning_add_from_seed(i, matching_index) > brightness_max){
                m_spinning_action[i] = ANIMATION_SPINNING_STATE_SUBSTRACT;
                animation_spinning_set_min_max_index(i, matching_index);
            }

        } else if(m_spinning_action[i] == ANIMATION_SPINNING_STATE_SUBSTRACT) {

            if(animation_spinning_substract(i, matching_index, brightness_min, brightness_max) < brightness_min) {
                m_spinning_action[i] = ANIMATION_SPINNING_STATE_ADD;
                animation_spinning_set_min_max_index(i, matching_index);
            }

        } else if(m_spinning_action[i] == ANIMATION_SPINNING_STATE_ADD) {

            if(animation_spinning_add(i, matching_index, brightness_min, brightness_max) > brightness_max) {
                m_spinning_action[i] = ANIMATION_SPINNING_STATE_SUBSTRACT;
                animation_spinning_set_min_max_index(i, matching_index);
            }
        }

        int brightness_target;

        switch (m_spinning_action[i])
        {
            case ANIMATION_SPINNING_STATE_SUBSTRACT_FROM_SEED:
                brightness_target = animation_spinning_substract_from_seed(i, matching_index);
                break;

            case ANIMATION_SPINNING_STATE_ADD_FROM_SEED:
                brightness_target = animation_spinning_add_from_seed(i, matching_index);
                break;

            case ANIMATION_SPINNING_STATE_SUBSTRACT:
                brightness_target = animation_spinning_substract(i, matching_index, brightness_min, brightness_max);
                break;

            case ANIMATION_SPINNING_STATE_ADD:
                brightness_target = animation_spinning_add(i, matching_index, brightness_min, brightness_max);
                break;

            default:
                brightness_target = 0;
                break;
        }

        m_leds[i].setHSV(hsv.hue, hsv.saturation, brightness_target);
    }

    FastLED.show();

    if (millis() > m_animation_start_time + animation_duration) {
        m_is_animation_running = false;
        Serial.println("[LedStrip] End spinning");
        return true;
    } else {
        return false;
    }        
}



void LedStrip::set_spinning_array(CHSV hsv, uint8_t brightness_min, uint8_t brightness_max) {
    //uint8_t brightness_seed[NB_LEDS];
    int center_index = (NB_LEDS/2) - 1;

    for(int i = 0; i<NB_LEDS; i++){
        uint8_t pixel_position_percent = map(i, 0, NB_LEDS-1, 0, 100);
        uint8_t brightness_diff = brightness_max - brightness_min;

        // This function is used to draw the spinning effect (can be linear or not)
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

}


void LedStrip::animation_spinning_set_pixel_color(uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max){

}