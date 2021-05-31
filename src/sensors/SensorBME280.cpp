#include <sensors/SensorBME280.h>

const int JSON_BUFFER_CAPACITY = 20;

SensorBME280::SensorBME280(int i2c_address, int measure_interval){
    m_i2c_address = i2c_address;
    m_measure_interval = measure_interval;

}

void SensorBME280::init() {
    if (! m_bme.begin(m_i2c_address)){
        Serial.println("[BME280] Not found. Retrying ...");
        while (1);
    }
}

bool SensorBME280::read() {
    if (millis() - m_last_measure_time > m_measure_interval){
        Serial.println("[BME280] Reading data");
        m_temperature = m_bme.readTemperature();
        m_humidity = m_bme.readHumidity();
        m_pressure = m_bme.readPressure() / 100.0F;

        m_last_measure_time = millis();

        return true;
    }
    else { 
        return false; 
    }

}

uint16_t SensorBME280::get_temperature(){ return m_temperature; }
uint16_t SensorBME280::get_humidity(){ return m_humidity; }
uint16_t SensorBME280::get_pressure(){ return m_pressure; }

