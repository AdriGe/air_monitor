#ifndef DEF_SENSOR_DS18B20
#define DEF_SENSOR_DS18B20

#include <DallasTemperature.h>
#include <Wire.h>

class SensorDS18B20 {
    public:
        SensorDS18B20(int pin, int measure_interval);
        void init();
        bool read();
        uint16_t get_temperature();

    private:
        int m_pin;
        uint m_measure_interval;
        unsigned long m_last_measure_time;
        OneWire* m_temperature_one_wire;
        DallasTemperature* m_dallas_temperature;
        uint16_t m_temperature;
};


#endif