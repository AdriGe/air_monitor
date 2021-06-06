#include <sensors/SensorSGP30.h>

using namespace std;

SensorSGP30::SensorSGP30(int measure_interval){
    m_measure_interval = measure_interval;

}

void SensorSGP30::init() {
    if (! m_sgp.begin()){
        Serial.println("[SGP30] Not found. Retrying ...");
        while (1);
    }
}

bool SensorSGP30::read() {
    if (millis() - m_last_measure_time > m_measure_interval){
        if (! m_sgp.IAQmeasure()) {
            Serial.println("[SGP30] Measurement failed");
        } else {
            Serial.println("[SGP30] Reading data");
            m_tvoc = m_sgp.TVOC;
            m_eco2 = m_sgp.eCO2;

            //get_json_object().prettyPrintTo(Serial);
            m_last_measure_time = millis();
        }

        return true;
    }
    else { 
        return false; 
    }
}


uint16_t SensorSGP30::get_tvoc(){ return m_tvoc; }
uint16_t SensorSGP30::get_eco2(){ return m_eco2; }
