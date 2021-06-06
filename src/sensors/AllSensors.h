#ifndef DEF_ALL_SENSORS
#define DEF_ALL_SENSORS

#include <Arduino.h>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <sensors/SensorPMSA003.h>
#include <sensors/SensorSGP30.h>
#include <sensors/SensorBME280.h>
#include <sensors/SensorDS18B20.h>

#define AQI_VERY_LOW_MIN 0
#define AQI_VERY_LOW_MAX 25
#define AQI_LOW_MIN 25
#define AQI_LOW_MAX 50
#define AQI_MEDIUM_MIN 50
#define AQI_MEDIUM_MAX 75
#define AQI_HIGH_MIN 75
#define AQI_HIGH_MAX 100
#define AQI_VERY_HIGH_MIN 100
#define AQI_VERY_HIGH_MAX 999

#define PM_2_5_VERY_LOW_MIN 0
#define PM_2_5_VERY_LOW_MAX 15
#define PM_2_5_LOW_MIN 15
#define PM_2_5_LOW_MAX 30
#define PM_2_5_MEDIUM_MIN 30
#define PM_2_5_MEDIUM_MAX 55
#define PM_2_5_HIGH_MIN 55
#define PM_2_5_HIGH_MAX 110
#define PM_2_5_VERY_HIGH_MIN 110
#define PM_2_5_VERY_HIGH_MAX 999

#define PM_10_0_VERY_LOW_MIN 0
#define PM_10_0_VERY_LOW_MAX 25
#define PM_10_0_LOW_MIN 25
#define PM_10_0_LOW_MAX 50
#define PM_10_0_MEDIUM_MIN 50
#define PM_10_0_MEDIUM_MAX 90
#define PM_10_0_HIGH_MIN 90
#define PM_10_0_HIGH_MAX 180
#define PM_10_0_VERY_HIGH_MIN 180
#define PM_10_0_VERY_HIGH_MAX 999

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
        uint16_t get_aqi();

        uint16_t get_mean_pm_1_0();
        uint16_t get_mean_pm_2_5();
        uint16_t get_mean_pm_10_0();
        uint16_t get_mean_temperature();
        uint16_t get_mean_humidity();
        uint16_t get_mean_pressure();
        uint16_t get_mean_tvoc();
        uint16_t get_mean_eco2();

        String get_json_object();

    private:
        SensorPMSA003* m_p_pmsa003;
        SensorSGP30* m_p_sgp30;
        SensorBME280* m_p_bme280;
        unsigned long m_last_measure_time_aqi;
        unsigned long m_measure_interval_aqi;
        unsigned int m_mean_nb_points;

        std::vector<uint16_t> m_vector_pm_1_0;
        std::vector<uint16_t> m_vector_pm_2_5;
        std::vector<uint16_t> m_vector_pm_10_0;
        std::vector<uint16_t> m_vector_tvoc;
        std::vector<uint16_t> m_vector_eco2;
        std::vector<uint16_t> m_vector_temperature;
        std::vector<uint16_t> m_vector_humidity;
        std::vector<uint16_t> m_vector_pressure;
        std::vector<uint16_t> m_vector_aqi;

        void handleFifo(uint16_t new_sensor_value, std::vector<uint16_t>* p_value_fifo);
        uint16_t get_aqi_pm_2_5(uint16_t pm_2_5);
        uint16_t get_aqi_pm_10_0(uint16_t pm_10_0);

};

#endif