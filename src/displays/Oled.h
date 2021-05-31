#ifndef DEF_OLED
#define DEF_OLD

#include <math.h>
#include <map>
#include <U8g2lib.h>
#include <fonts/_muar_stencil/Muar_Stencil_44.h>
#include <fonts/_muar_stencil/Muar_Stencil_46.h>
#include <fonts/_muar_stencil/Muar_Stencil_48.h>
#include <fonts/_muar_stencil/Muar_Stencil_50.h>
#include <fonts/_muar_stencil/Muar_Stencil_52.h>
#include <fonts/_muar_stencil/Muar_Stencil_54.h>
#include <fonts/_muar_stencil/Muar_Stencil_56.h>
#include <fonts/_muar_stencil/Muar_Stencil_58.h>
#include <fonts/_muar_stencil/Muar_Stencil_60.h>
#include <fonts/_muar_stencil/Muar_Stencil_62.h>
#include <fonts/_muar_stencil/Muar_Stencil_64.h>
#include <sensors/AllSensors.h>

class Oled {
    public:
        Oled();
        void init();
        void drawStr(int x, int y, char* str);
        bool translateLeft(char* str, uint16_t enter_duration, uint16_t static_duration, uint16_t exit_duration, uint16_t blank_duration);
        void loop(AllSensors* p_all_sensors, 
                  uint16_t measure_enter_duration, 
                  uint16_t measure_static_duration, 
                  uint16_t measure_exit_duration, 
                  uint16_t measure_blank_duration, 
                  uint16_t value_enter_duration, 
                  uint16_t value_static_duration, 
                  uint16_t value_exit_duration, 
                  uint16_t value_blank_duration);

    private:
        U8G2_SH1106_128X64_NONAME_F_HW_I2C* m_p_u8g2;
        uint16_t m_screen_width;
        uint16_t m_screen_height;
        unsigned long m_animation_start_time;
        bool m_is_animation_running;
        int m_current_loop_state;
        char m_str_to_display[4];
        bool m_is_printing_sensor_value;
        std::map<int, char*> m_loop_mapping;

        void enterLeft(char* str, uint16_t enter_duration);
        void exitLeft(char* str, uint16_t enter_duration, uint16_t static_duration, uint16_t exit_duration);
        void callBackLoopMeasure(char* str_to_display, uint16_t measure_enter_duration, uint16_t measure_static_duration, uint16_t measure_exit_duration, uint16_t measure_blank_duration);
        void callBackLoopValue(uint16_t int_to_display, uint16_t value_enter_duration, uint16_t value_static_duration, uint16_t value_exit_duration, uint16_t value_blank_duration);
};

#endif