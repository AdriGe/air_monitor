#include <sensors/SensorPMSA003.h>

using namespace std;

SensorPMSA003::SensorPMSA003(int pin_rx, int pin_tx, int measure_interval)
{
    m_pin_tx = pin_tx;
    m_pin_rx = pin_rx;
    m_pms_timer_interval = PMS_READ_DELAY;
    m_pm_1_0 = 0;
    m_pm_2_5 = 0;
    m_pm_10_0 = 0;

    m_p_pms_serial = new SoftwareSerial(14,12);
    m_p_pms = new PMS(*m_p_pms_serial);
}


void SensorPMSA003::init() {
    m_p_pms_serial->begin(PMS::BAUD_RATE);
    m_p_pms->passiveMode();
    m_p_pms->wakeUp();
}


bool SensorPMSA003::read() {
    static uint32_t timerLast = 0;

    uint32_t timerNow = millis();
    if (timerNow - timerLast >= m_pms_timer_interval) {
        timerLast = timerNow;
        timer_callback();
        m_pms_timer_interval = m_pms_timer_interval == PMS_READ_DELAY ? PMS_READ_INTERVAL : PMS_READ_DELAY;

        return true;
    }
    else { 
        return false; 
    }
}

void SensorPMSA003::get_data()
{
    PMS::DATA data;

    // Clear buffer (removes potentially old data) before read. Some data could have been also sent before switching to passive mode.
    while (Serial.available()) { Serial.read(); }

    Serial.println("[PMSA003] Send read request...");
    m_p_pms->requestRead();

    Serial.println("[PMSA003] Reading data...");
    if (m_p_pms->readUntil(data))
    {
        m_pm_1_0 = data.PM_AE_UG_1_0;  
        m_pm_2_5 = data.PM_AE_UG_2_5;
        m_pm_10_0 = data.PM_AE_UG_10_0;
    }
    else
    {
        Serial.println("[PMSA003] No data.");
    }
}


void SensorPMSA003::timer_callback() {
    if (m_pms_timer_interval == PMS_READ_DELAY)
    {
        get_data();
        Serial.println("[PMSA003] Going to sleep.");
        m_p_pms->sleep();
    }
    else {
        Serial.println("[PMSA003] Waking up.");
        m_p_pms->wakeUp();
    }
}

uint16_t SensorPMSA003::get_pm_1_0() { return m_pm_1_0; }
uint16_t SensorPMSA003::get_pm_2_5() { return m_pm_2_5; }
uint16_t SensorPMSA003::get_pm_10_0() { return m_pm_10_0; }