#include <sensors/AllSensors.h>

using namespace std;

AllSensors::AllSensors(
    SensorPMSA003* p_pmsa003,
    SensorSGP30* p_sgp30,
    SensorBME280* p_bme280,
    unsigned int mean_nb_points)
{
    m_p_pmsa003 = p_pmsa003;
    m_p_sgp30 = p_sgp30;
    m_p_bme280 = p_bme280;
    m_mean_nb_points = mean_nb_points;
}


void AllSensors::init() {
    m_p_pmsa003->init();
    m_p_sgp30->init();
    m_p_bme280->init();
}


bool AllSensors::read() {
    bool is_pmsa003_new_values = m_p_pmsa003->read();
    bool is_sgp30_new_values = m_p_sgp30->read();
    bool is_bme280_new_values = m_p_bme280->read();

    //Fill FIFO
    if (is_pmsa003_new_values) {
        handleFifo(m_p_pmsa003->get_pm_1_0(), &m_vector_pm_1_0);
        handleFifo(m_p_pmsa003->get_pm_2_5(), &m_vector_pm_2_5);
        handleFifo(m_p_pmsa003->get_pm_10_0(), &m_vector_pm_10_0);
    }

    if (is_sgp30_new_values) {
        handleFifo(m_p_sgp30->get_tvoc(), &m_vector_tvoc);
        handleFifo(m_p_sgp30->get_eco2(), &m_vector_eco2);
    }

    if (is_bme280_new_values) {
        handleFifo(m_p_bme280->get_temperature(), &m_vector_temperature);
        handleFifo(m_p_bme280->get_humidity(), &m_vector_humidity);
        handleFifo(m_p_bme280->get_pressure(), &m_vector_pressure);
    }

    if (is_pmsa003_new_values ||
        is_sgp30_new_values ||
        is_bme280_new_values)
    {
        return true;
    }
    else {
        return false;
    }
}

void AllSensors::handleFifo(uint16_t new_sensor_value, vector<uint16_t>* p_value_fifo) {
    //Keep fixed number of values in vectors
    if (p_value_fifo->size() == m_mean_nb_points) {
        p_value_fifo->erase(p_value_fifo->begin());
    }

    p_value_fifo->push_back(new_sensor_value);
}

uint16_t AllSensors::get_pm_1_0() { return m_p_pmsa003->get_pm_1_0(); }
uint16_t AllSensors::get_pm_2_5() { return m_p_pmsa003->get_pm_2_5(); }
uint16_t AllSensors::get_pm_10_0() { return m_p_pmsa003->get_pm_10_0(); }
uint16_t AllSensors::get_tvoc() { return m_p_sgp30->get_tvoc(); }
uint16_t AllSensors::get_eco2() { return m_p_sgp30->get_eco2(); }
uint16_t AllSensors::get_temperature() { return m_p_bme280->get_temperature(); }
uint16_t AllSensors::get_humidity() { return m_p_bme280->get_humidity(); }
uint16_t AllSensors::get_pressure() { return m_p_bme280->get_pressure(); }

uint16_t AllSensors::get_mean_pm_1_0() { return accumulate(m_vector_pm_1_0.begin(), m_vector_pm_1_0.end(), 0)/m_mean_nb_points; }
uint16_t AllSensors::get_mean_pm_2_5() { return accumulate(m_vector_pm_2_5.begin(), m_vector_pm_2_5.end(), 0)/m_mean_nb_points; }
uint16_t AllSensors::get_mean_pm_10_0() { return accumulate(m_vector_pm_10_0.begin(), m_vector_pm_10_0.end(), 0)/m_mean_nb_points; }
uint16_t AllSensors::get_mean_tvoc() { return accumulate(m_vector_tvoc.begin(), m_vector_tvoc.end(), 0)/m_mean_nb_points; }
uint16_t AllSensors::get_mean_eco2() { return accumulate(m_vector_eco2.begin(), m_vector_eco2.end(), 0)/m_mean_nb_points; }
uint16_t AllSensors::get_mean_temperature() { return accumulate(m_vector_temperature.begin(), m_vector_temperature.end(), 0)/m_mean_nb_points; }
uint16_t AllSensors::get_mean_humidity() { return accumulate(m_vector_humidity.begin(), m_vector_humidity.end(), 0)/m_mean_nb_points; }
uint16_t AllSensors::get_mean_pressure() { return accumulate(m_vector_pressure.begin(), m_vector_pressure.end(), 0)/m_mean_nb_points; }

uint16_t AllSensors::get_aqi() {
    uint16_t pm_10_0 = get_mean_pm_10_0();

    if      (pm_10_0 > 180)  { return 5; }
    else if (pm_10_0 >  90)  { return 4; }
    else if (pm_10_0 >  50)  { return 3; }
    else if (pm_10_0 >  25)  { return 2; }
    else                     { return 1; }
}


String AllSensors::get_json_object() {
    String json_object = ""
        "{"
            "\"pmsa003\":{"
                "\"pm_1_0\":{"
                    "\"value\":" + String(m_p_pmsa003->get_pm_1_0()) + ","
                    "\"unit\":\"ug/m3\""
                "},"
                "\"pm_2_5\":{"
                    "\"value\":" + String(m_p_pmsa003->get_pm_2_5()) + ","
                    "\"unit\":\"ug/m3\""
                "},"
                "\"pm_10_0\":{"
                    "\"value\":" + String(m_p_pmsa003->get_pm_10_0()) + ","
                    "\"unit\":\"ug/m3\""
                "}"
            "},"
            "\"sgp30\":{"
                "\"tvoc\":{"
                    "\"value\":" + String(m_p_sgp30->get_tvoc()) + ","
                    "\"unit\":\"ppb\""
                "},"
                "\"eco2\":{"
                    "\"value\":" + String(m_p_sgp30->get_eco2()) + ","
                    "\"unit\":\"ppm\""
                "}"
            "},"
            "\"bme280\":{"
                "\"temperature\":{"
                    "\"value\":" + String(m_p_bme280->get_temperature()) + ","
                    "\"unit\":\"°C\""
                "},"
                "\"humidity\":{"
                    "\"value\":" + String(m_p_bme280->get_humidity()) + ","
                    "\"unit\":\"%\""
                "},"
                "\"pressure\":{"
                    "\"value\":" + String(m_p_bme280->get_pressure()) + ","
                    "\"unit\":\"hPa\""
                "}"
            "}"
        "}";
    

    return json_object;
}
