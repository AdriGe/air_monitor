#ifndef DEF_ALL_SENSORS
#define DEF_ALL_SENSORS

#include <Arduino.h>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <sensors/SensorPMSA003.h>
#include <sensors/SensorSGP30.h>
#include <sensors/SensorBME280.h>
#include <sensors/SensorDS18B20.h>

class AllSensors{
    public:
        AllSensors(
            SensorPMSA003* m_p_pmsa003,
            SensorSGP30* m_p_sgp30,
            SensorBME280* m_p_bme280,
            unsigned int mean_nb_points
        );
        void init();
        bool read();

        uint16_t get_pm_1_0();
        uint16_t get_pm_2_5();
        uint16_t get_pm_10_0();
        uint16_t get_tvoc();
        uint16_t get_eco2();
        uint16_t get_temperature();
        uint16_t get_humidity();
        uint16_t get_pressure();

        uint16_t get_mean_pm_1_0();
        uint16_t get_mean_pm_2_5();
        uint16_t get_mean_pm_10_0();
        uint16_t get_mean_temperature();
        uint16_t get_mean_humidity();
        uint16_t get_mean_pressure();
        uint16_t get_mean_tvoc();
        uint16_t get_mean_eco2();

        uint16_t get_aqi();
        String get_json_object();

    private:
        SensorPMSA003* m_p_pmsa003;
        SensorSGP30* m_p_sgp30;
        SensorBME280* m_p_bme280;
        unsigned int m_mean_nb_points;

        std::vector<uint16_t> m_vector_pm_1_0;
        std::vector<uint16_t> m_vector_pm_2_5;
        std::vector<uint16_t> m_vector_pm_10_0;
        std::vector<uint16_t> m_vector_tvoc;
        std::vector<uint16_t> m_vector_eco2;
        std::vector<uint16_t> m_vector_temperature;
        std::vector<uint16_t> m_vector_humidity;
        std::vector<uint16_t> m_vector_pressure;

        void handleFifo(uint16_t new_sensor_value, std::vector<uint16_t>* p_value_fifo);

};

#endif