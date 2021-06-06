#include <sensors/SensorDS18B20.h>

using namespace std;

SensorDS18B20::SensorDS18B20(int pin, int measure_interval){
    m_pin = pin;
    m_measure_interval = measure_interval;

    m_temperature_one_wire = new OneWire(m_pin);
    m_dallas_temperature = new DallasTemperature(m_temperature_one_wire);
    //OneWire temperatureOneWire(TEMPERATURE_PIN);
    //DallasTemperature temperature(&temperatureOneWire);

}

void SensorDS18B20::init() {
    m_dallas_temperature->begin();
}

bool SensorDS18B20::read() {
    if (millis() - m_last_measure_time > m_measure_interval){
        Serial.println("[DS18B20] Reading data");
        
        m_temperature = m_dallas_temperature->getTempCByIndex(0);
        m_dallas_temperature->requestTemperatures();
        
        m_last_measure_time = millis();

        return true;
    } 
    else { 
        return false; 
    }

}


uint16_t SensorDS18B20::get_temperature(){ return m_temperature; }

