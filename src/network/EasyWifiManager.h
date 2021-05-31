#ifndef DEF_EASY_WIFI_MANAGER
#define DEF_EASY_WIFI_MANAGER

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <config/config.h>

class EasyWifiManager {
    public:
        EasyWifiManager();
        void init();
        void init_wifi_manager();
        void load_config();
        void save_config();
        void reset_config();
        String get_ssid();
        String get_psk();
        char* get_mqtt_server();
        char* get_mqtt_port();
        char* get_mqtt_user();
        char* get_mqtt_password();
        char* get_mqtt_topic();
        

    private:
        // Default MQTT values for WifiManager configuration portal
        String m_ssid;
        String m_psk;
        char mqtt_server[40];
        char mqtt_port[6];
        char mqtt_user[40];
        char mqtt_password[70];
        char mqtt_topic[255];

};


#endif