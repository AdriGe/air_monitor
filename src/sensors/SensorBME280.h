#ifndef DEF_SENSOR_BME280
#define DEF_SENSOR_BME280

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h> //https://lastminuteengineers.com/bme280-arduino-tutorial/

class SensorBME280 {
    public:
        SensorBME280(int i2c_address, int measure_interval);
        void init();
        bool read();
        uint16_t get_temperature();
        uint16_t get_humidity();
        uint16_t get_pressure();

    private:
        int m_i2c_address;
        uint m_measure_interval;
        unsigned long m_last_measure_time;
        Adafruit_BME280 m_bme;
        uint16_t m_temperature;
        uint16_t m_humidity;
        uint16_t m_pressure;
};


#endif