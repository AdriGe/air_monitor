#include <displays/Oled.h>

using namespace std;

Oled::Oled() {
    m_p_u8g2 = new U8G2_SH1106_128X64_NONAME_F_HW_I2C(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
    m_is_animation_running = false;
    m_screen_width = m_p_u8g2->getDisplayWidth();
    m_screen_height = m_p_u8g2->getDisplayHeight();
    m_current_loop_state = 0;
    m_is_printing_sensor_value = false;

    char m_loop_mapping_pm_1_0[] = "PM1.0";
    char m_loop_mapping_pm_2_5[] = "PM2.5";
    char m_loop_mapping_pm_10[] = "PM10";
    char m_loop_mapping_tvoc[] = "TVOC";
    char m_loop_mapping_eco2[] = "eCO2";
    char m_loop_mapping_temperature[] = "Temp";
    char m_loop_mapping_humidity[] = "H%";

    m_loop_mapping[0] = m_loop_mapping_pm_1_0;
    m_loop_mapping[2] = m_loop_mapping_pm_2_5;
    m_loop_mapping[4] = m_loop_mapping_pm_10;
    m_loop_mapping[6] = m_loop_mapping_tvoc;
    m_loop_mapping[8] = m_loop_mapping_eco2;
    m_loop_mapping[10] = m_loop_mapping_temperature;
    m_loop_mapping[12] = m_loop_mapping_humidity;
}

void Oled::init() {
    m_p_u8g2->begin();
    m_p_u8g2->setFont(Muar_Stencil_44);
    m_p_u8g2->setFontPosTop();
}


void Oled::drawStr(int x, int y, char* str) {   
    m_p_u8g2->firstPage();
    do {
            m_p_u8g2->drawStr(x,y,str);
    } while (m_p_u8g2->nextPage());
}


void Oled::loop(AllSensors* p_all_sensors, 
                uint16_t measure_enter_duration, 
                uint16_t measure_static_duration, 
                uint16_t measure_exit_duration, 
                uint16_t measure_blank_duration, 
                uint16_t value_enter_duration, 
                uint16_t value_static_duration, 
                uint16_t value_exit_duration, 
                uint16_t value_blank_duration)
{
    switch (m_current_loop_state)
    {
    case 0: callBackLoopMeasure(m_loop_mapping[m_current_loop_state], measure_enter_duration, measure_static_duration, measure_exit_duration, measure_blank_duration); break;
    case 1: callBackLoopValue(p_all_sensors->get_mean_pm_1_0(), value_enter_duration, value_static_duration, value_exit_duration, value_blank_duration); break;

    case 2: callBackLoopMeasure(m_loop_mapping[m_current_loop_state], measure_enter_duration, measure_static_duration, measure_exit_duration, measure_blank_duration); break;
    case 3: callBackLoopValue(p_all_sensors->get_mean_pm_2_5(), value_enter_duration, value_static_duration, value_exit_duration, value_blank_duration); break;

    case 4: callBackLoopMeasure(m_loop_mapping[m_current_loop_state], measure_enter_duration, measure_static_duration, measure_exit_duration, measure_blank_duration); break;
    case 5: callBackLoopValue(p_all_sensors->get_mean_pm_10_0(), value_enter_duration, value_static_duration, value_exit_duration, value_blank_duration); break;

    case 6: callBackLoopMeasure(m_loop_mapping[m_current_loop_state], measure_enter_duration, measure_static_duration, measure_exit_duration, measure_blank_duration); break;
    case 7: callBackLoopValue(p_all_sensors->get_tvoc(), value_enter_duration, value_static_duration, value_exit_duration, value_blank_duration); break;

    case 8: callBackLoopMeasure(m_loop_mapping[m_current_loop_state], measure_enter_duration, measure_static_duration, measure_exit_duration, measure_blank_duration); break;
    case 9: callBackLoopValue(p_all_sensors->get_eco2(), value_enter_duration, value_static_duration, value_exit_duration, value_blank_duration); break;

    case 10: callBackLoopMeasure(m_loop_mapping[m_current_loop_state], measure_enter_duration, measure_static_duration, measure_exit_duration, measure_blank_duration); break;
    case 11: callBackLoopValue(p_all_sensors->get_temperature(), value_enter_duration, value_static_duration, value_exit_duration, value_blank_duration); break;

    case 12: callBackLoopMeasure(m_loop_mapping[m_current_loop_state], measure_enter_duration, measure_static_duration, measure_exit_duration, measure_blank_duration); break;
    case 13: callBackLoopValue(p_all_sensors->get_humidity(), value_enter_duration, value_static_duration, value_exit_duration, value_blank_duration); break;

    default: m_current_loop_state = 0; break;
    }
}


void Oled::callBackLoopMeasure(char* str_to_display, uint16_t measure_enter_duration, uint16_t measure_static_duration, uint16_t measure_exit_duration, uint16_t measure_blank_duration){
    m_p_u8g2->setFont(Muar_Stencil_48);
    if (translateLeft(str_to_display, measure_enter_duration, measure_static_duration, measure_exit_duration, measure_blank_duration)) {
        m_current_loop_state++;
    }
}


void Oled::callBackLoopValue(uint16_t int_to_display, uint16_t value_enter_duration, uint16_t value_static_duration, uint16_t value_exit_duration, uint16_t value_blank_duration){
    m_p_u8g2->setFont(Muar_Stencil_64);
    if(!m_is_printing_sensor_value) {
        if (int_to_display > 999) { int_to_display = 999; }
        itoa(int_to_display, m_str_to_display, 10);
    }
        
    m_is_printing_sensor_value = true;

    if (translateLeft(m_str_to_display, value_enter_duration, value_static_duration, value_exit_duration, value_blank_duration)) {
        m_is_printing_sensor_value = false;
        m_current_loop_state++;
    }
}


bool Oled::translateLeft(char* str, uint16_t enter_duration, uint16_t static_duration, uint16_t exit_duration, uint16_t blank_duration) {
    if (!m_is_animation_running) {
        Serial.println("[Oled] Start of translateLeft");
        m_is_animation_running = true;
        m_animation_start_time = millis();
    }

    unsigned long animation_time = millis() - m_animation_start_time;
    unsigned long animation_duration = enter_duration + static_duration + exit_duration + blank_duration;
    //int x_position_center = (m_screen_width - m_p_u8g2->getStrWidth(str)) / 2;
    //int y_position_center = (m_screen_height-m_p_u8g2->getMaxCharHeight())/2+8;

    if(animation_time<enter_duration) {
        enterLeft(str, enter_duration);

    } else if (animation_time > enter_duration + static_duration && animation_time < animation_duration - blank_duration) {
        exitLeft(str, enter_duration, static_duration, exit_duration);
    }
    
    if (millis() > m_animation_start_time + animation_duration) {
        m_is_animation_running = false;
        Serial.println("[Oled] End of translateLeft");
        return true;
    } else {
        return false;
    }
}


void Oled::enterLeft(char* str, uint16_t enter_duration) {
    long animation_time = millis() - m_animation_start_time;
    int x_start_position = m_screen_width;
    int x_final_position = (m_screen_width - m_p_u8g2->getStrWidth(str)) / 2;
    int y_position_center = (m_screen_height-m_p_u8g2->getMaxCharHeight())/2+8;
    
    // Get current percentage of time elapsed in animation
    long percent_animation_time = Arduino_h::map(animation_time, 0, enter_duration, 0, 100);
    
    // Map the current percentage of time elapsed in animation with the string percentage position using a "custom" function
    // The function curve affects the translate effet.
    // Given [0 ; 100] as the possible input range, it must returns values in the same range (ie [0 ; 100]).
    // The function can be found using https://www.dcode.fr/function-equation-finder (so far, best results were found with "Logarithmic Using Curve Fitting")
    long percent_animation_position = 33.6556 * log( (471.46 * percent_animation_time) + 2395.53 ) - 259.802;
    if (percent_animation_position > 100) { percent_animation_position = 100; }

    // Map the percentage positition with the corresponding value on the screen
    long x_position_value = Arduino_h::map(percent_animation_position, 0, 100, x_start_position, x_final_position);
    
    drawStr(x_position_value, y_position_center, str);
}


void Oled::exitLeft(char* str, uint16_t enter_duration, uint16_t static_duration, uint16_t exit_duration) {
    long animation_time = millis() - m_animation_start_time - enter_duration - static_duration;
    int x_start_position = (m_screen_width - m_p_u8g2->getStrWidth(str)) / 2;
    int x_final_position = -(m_p_u8g2->getStrWidth(str) + 10); //Add 10 because getStrWidth is not precise
    int y_position_center = (m_screen_height-m_p_u8g2->getMaxCharHeight())/2+8;

    // Get current percentage of time elapsed in animation
    long percent_animation_time = Arduino_h::map(animation_time, 0, exit_duration, 0, 100);

    // Map the percentage of time elapsed with the corresponding value on the screen
    long x_position_value = Arduino_h::map(percent_animation_time, 0, 100, x_start_position, x_final_position);

    drawStr(x_position_value, y_position_center, str);
}



