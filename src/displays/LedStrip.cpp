#include <displays/LedStrip.h>

using namespace std;

LedStrip::LedStrip()
{
    FastLED.addLeds<NEOPIXEL, LED_STRIP_DATA_PIN>(m_leds, NB_LEDS);
    m_is_animation_running = false;
    m_current_animation = CURRENT_ANIMATION_NONE;

    m_color_palette = CRGBPalette16(
        CRGB(0,229,188),
        CRGB(84,229,134),
        CRGB(167,230,80),
        CRGB(251,230,26),
        CRGB(252,206,20),
        CRGB(253,183,14),
        CRGB(254,159,8),
        CRGB(255,135,2),
        CRGB(250,119,21),
        CRGB(244,103,40),
        CRGB(239,86,59),
        CRGB(233,70,78),
        CRGB(222,70,117),
        CRGB(212,69,156),
        CRGB(201,69,195),
        CRGB(190,68,234)
    );
}


void LedStrip::set_color(CRGB rgb) {
    for(int i = 0; i<NB_LEDS; i++) {
        m_leds[i].setRGB(rgb.red, rgb.green, rgb.blue);
    }

    FastLED.show();
}

void LedStrip::set_color(uint8_t red, uint8_t green, uint8_t blue) {
    for(int i = 0; i<NB_LEDS; i++) {
        m_leds[i].setRGB(red, green, blue);
    }

    FastLED.show();
}


void LedStrip::set_brightness(uint8_t brightness) {
    FastLED.setBrightness(brightness);
}


void LedStrip::loop(AllSensors* p_all_sensors) {
    uint16_t aqi = p_all_sensors->get_aqi();
    CHSV color_aqi = get_color_from_aqi(aqi);

    if      (aqi>AQI_VERY_HIGH_MIN) { set_animation_spin(color_aqi, 50, 255, 3000); }
    else if (aqi>AQI_HIGH_MIN)      { set_animation_spin(color_aqi, 50, 255, 3000); }
    else if (aqi>AQI_MEDIUM_MIN)    { set_animation_breath(color_aqi, 0, 255, 5000, 1000); }
    else if (aqi>AQI_LOW_MIN)       { set_animation_breath(color_aqi, 0, 255, 5000, 1000); }
    else                            { set_animation_breath(color_aqi, 0, 255, 5000, 1000); }
}

CHSV LedStrip::get_color_from_aqi(uint16_t aqi) {
    // HSV -> RGB
    //CHSV my_chsv(34, 255, 255);
    //CRGB my_crgb;
    //hsv2rgb_rainbow(my_chsv, my_crgb);
    //
    // RGB -> HSV
    //CRGB my_crgb(128, 128, 128);
    //CHSV my_hsv = rgb2hsv_approximate(my_crgb);
    //
    // Get Gradient
    //https://mistic100.github.io/tinygradient/
    //https://github.com/FastLED/FastLED/wiki/Gradient-color-palettes

    CRGB color_palette_index = map(aqi, AQI_VERY_LOW_MIN, AQI_VERY_HIGH_MIN, 0, 15);
    CHSV aqi_color = rgb2hsv_approximate(m_color_palette[color_palette_index]);
    return aqi_color;


    //if      (aqi>AQI_VERY_HIGH_MIN) { return CHSV(HUE_PURPLE, 255, 255); }
    //else if (aqi>AQI_HIGH_MIN)      { return CHSV(HUE_RED, 255, 255); }
    //else if (aqi>AQI_MEDIUM_MIN)    { return CHSV(HUE_ORANGE, 255, 255); }
    //else if (aqi>AQI_LOW_MIN)       { return CHSV(HUE_YELLOW, 255, 255); }
    //else                            { return CHSV(HUE_GREEN, 255, 255); }
}


bool LedStrip::set_animation_breath(CHSV hsv, uint8_t brightness_min, uint8_t brightness_max, unsigned long animation_duration, unsigned long animation_duration_low_static){
    if (!m_is_animation_running || m_current_animation != CURRENT_ANIMATION_BREATHING) {
        Serial.println("[LEDStrip] Start animation (Breath)");
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
        Serial.println("[LedStrip] End animation (Breath)");
        return true;
    } else {
        return false;
    }        
}


bool LedStrip::set_animation_spin(CHSV hsv, uint8_t brightness_min, uint8_t brightness_max, unsigned long animation_duration) {

    if (!m_is_animation_running || m_current_animation != CURRENT_ANIMATION_SPINNING) {
        Serial.println("[LEDStrip] Start animation (Spin)");
        m_is_animation_running = true;
        m_current_animation = CURRENT_ANIMATION_SPINNING;
        m_animation_start_time = millis();
        animation_spin_set_array(hsv, brightness_min, brightness_max);
    }

    int brightness_difference = brightness_max - brightness_min;
    unsigned long animation_time = millis() - m_animation_start_time;
    int animation_percent = map(animation_time, 0, animation_duration, 0, 100);
    int matching_index = map(animation_percent, 0, 100, 0, brightness_difference-1);

    for(int i = 0; i<NB_LEDS; i++){
        animation_spin_set_action(i, matching_index, brightness_min, brightness_max);
        animation_spin_set_pixel_color(hsv, i, matching_index, brightness_min, brightness_max);
    }

    FastLED.show();

    if (millis() > m_animation_start_time + animation_duration) {
        m_is_animation_running = false;
        m_current_animation = CURRENT_ANIMATION_NONE;
        Serial.println("[LedStrip] End animation (Spin)");
        return true;
    } else {
        return false;
    }        
}


void LedStrip::animation_spin_set_array(CHSV hsv, uint8_t brightness_min, uint8_t brightness_max) {
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
        
        m_spin_seed[i] = pixel_brightness;

        if(i<=center_index) {
            m_spin_action[i] = ANIMATION_SPIN_STATE_SUBSTRACT_FROM_SEED;
        } else {
            m_spin_action[i] = ANIMATION_SPIN_STATE_ADD_FROM_SEED;
        }
    }
}


uint16_t LedStrip::animation_spin_substract_from_seed(uint16_t current_pixel, uint8_t current_index) {
    return m_spin_seed[current_pixel] - (2*current_index);
}


uint16_t LedStrip::animation_spin_add_from_seed(uint16_t current_pixel, uint8_t current_index) {
    return m_spin_seed[current_pixel] + (2*current_index);
}


uint16_t LedStrip::animation_spin_substract(uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max) {
    return brightness_max - ( 2 * ( current_index - m_spin_min_max_index[current_pixel] ) );
}


uint16_t LedStrip::animation_spin_add(uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max) {
    return brightness_min + ( 2 * ( current_index - m_spin_min_max_index[current_pixel] ) );
}


void LedStrip::animation_spin_set_min_max_index(uint16_t current_pixel, uint8_t current_index){
    if(current_index - 1 < 0){
        m_spin_min_max_index[current_pixel] = 0;
    } else {
        m_spin_min_max_index[current_pixel] = current_index - 1;
    }
}


void LedStrip::animation_spin_set_action(uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max){

    if((m_spin_action[current_pixel] == ANIMATION_SPIN_STATE_SUBSTRACT_FROM_SEED) &&
       (animation_spin_substract_from_seed(current_pixel, current_index) < brightness_min)) 
    {
        m_spin_action[current_pixel] = ANIMATION_SPIN_STATE_ADD;
        animation_spin_set_min_max_index(current_pixel, current_index);

    } 
    else if((m_spin_action[current_pixel] == ANIMATION_SPIN_STATE_ADD_FROM_SEED) &&
            (animation_spin_add_from_seed(current_pixel, current_index) > brightness_max))
    {
        m_spin_action[current_pixel] = ANIMATION_SPIN_STATE_SUBSTRACT;
        animation_spin_set_min_max_index(current_pixel, current_index);

    } 
    else if((m_spin_action[current_pixel] == ANIMATION_SPIN_STATE_SUBSTRACT) && 
            (animation_spin_substract(current_pixel, current_index, brightness_min, brightness_max) < brightness_min))
    {
        m_spin_action[current_pixel] = ANIMATION_SPIN_STATE_ADD;
        animation_spin_set_min_max_index(current_pixel, current_index);

    } 
    else if((m_spin_action[current_pixel] == ANIMATION_SPIN_STATE_ADD) && 
            (animation_spin_add(current_pixel, current_index, brightness_min, brightness_max) > brightness_max)) 
    {
        m_spin_action[current_pixel] = ANIMATION_SPIN_STATE_SUBSTRACT;
        animation_spin_set_min_max_index(current_pixel, current_index);
    }
}


void LedStrip::animation_spin_set_pixel_color(CHSV hsv, uint16_t current_pixel, uint8_t current_index, uint8_t brightness_min, uint8_t brightness_max){
        int brightness_target;

        switch (m_spin_action[current_pixel])
        {
            case ANIMATION_SPIN_STATE_SUBSTRACT_FROM_SEED:
                brightness_target = animation_spin_substract_from_seed(current_pixel, current_index);
                break;

            case ANIMATION_SPIN_STATE_ADD_FROM_SEED:
                brightness_target = animation_spin_add_from_seed(current_pixel, current_index);
                break;

            case ANIMATION_SPIN_STATE_SUBSTRACT:
                brightness_target = animation_spin_substract(current_pixel, current_index, brightness_min, brightness_max);
                break;

            case ANIMATION_SPIN_STATE_ADD:
                brightness_target = animation_spin_add(current_pixel, current_index, brightness_min, brightness_max);
                break;

            default:
                brightness_target = 0;
                break;
        }

        m_leds[current_pixel].setHSV(hsv.hue, hsv.saturation, brightness_target);
}