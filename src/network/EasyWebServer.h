#ifndef DEF_EASY_WEB_SERVER
#define DEF_EASY_WEB_SERVER

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <network/EasyWifiManager.h>
#include <sensors/AllSensors.h>

class EasyWebServer {
    public:
        EasyWebServer(uint16_t port, EasyWifiManager* p_easy_wifi_manager, AllSensors* p_all_sensors);
        void init();
        void handle_client();
        void handle_resetWifi();
        void handle_getJsonValues();
        void handle_NotFound();

    private:
        uint16_t m_port;
        ESP8266WebServer* m_p_server;
        EasyWifiManager* m_p_easy_wifi_manager;
        AllSensors* m_p_all_sensors;
};

#endif