#ifndef DEF_SENSOR_PMSA003
#define DEF_SENSOR_PMSA003

#include <SoftwareSerial.h>
#include <PMS.h>
#include <ArduinoJson.h>                    // https://github.com/bblanchon/ArduinoJson

// PMS_READ_INTERVAL (4:30 min) and PMS_READ_DELAY (30 sec) CAN'T BE EQUAL! Values are also used to detect sensor state.
static const uint32_t PMS_READ_INTERVAL = 2000;
static const uint32_t PMS_READ_DELAY = 3000;

class SensorPMSA003 {
    public:
        SensorPMSA003(int pin_tx, int pin_rx, int measure_interval);
        void init();
        bool read();
        void timer_callback();
        void get_data();
        uint16_t get_pm_1_0();
        uint16_t get_pm_2_5();
        uint16_t get_pm_10_0();

    private:
        int m_pin_tx;
        int m_pin_rx;
        
        SoftwareSerial* m_p_pms_serial;
        PMS* m_p_pms;
        
        uint32_t m_pms_timer_interval;
        uint16_t m_pm_1_0;
        uint16_t m_pm_2_5;
        uint16_t m_pm_10_0;
};


#endif