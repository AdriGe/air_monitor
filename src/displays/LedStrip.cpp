#include <displays/LedStrip.h>

using namespace std;

LedStrip::LedStrip()
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(m_leds, NB_LEDS);
    m_effect_breath_last_exec_time = millis();
    m_effect_breath_iteration = 0;
    m_effect_breath_delay = 30;
}

void LedStrip::set_color(CRGB color) {
    for(int i=0; i<NB_LEDS; i++){
        m_leds[i] = color;
    }
    FastLED.show();
}

void LedStrip::set_color(uint8_t red, uint8_t green, uint8_t blue) {
    for(int i=0; i<NB_LEDS; i++){
        m_leds[i].setRGB(red, green, blue);
    }
    FastLED.show();
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