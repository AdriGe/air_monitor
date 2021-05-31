#ifndef DEF_SENSOR_SGP30
#define DEF_SENSOR_SGP30

#include <Adafruit_Sensor.h>
#include <Adafruit_SGP30.h> //https://github.com/adafruit/Adafruit_SGP30/blob/master/examples/sgp30test/sgp30test.ino
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

class SensorSGP30 {
    public:
        SensorSGP30(int measure_interval);
        void init();
        bool read();
        uint16_t get_tvoc();
        uint16_t get_eco2();

    private:
        uint m_measure_interval;
        unsigned long m_last_measure_time;
        Adafruit_SGP30 m_sgp;
        uint16_t m_tvoc;
        uint16_t m_eco2;
};


#endif