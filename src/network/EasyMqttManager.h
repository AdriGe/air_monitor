#ifndef DEF_EASY_MQTT_MANAGER
#define DEF_EASY_MQTT_MANAGER

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>
#include <network/EasyWifiManager.h>

class EasyMqttManager{
    public:
        EasyMqttManager(char* server, int port, char* user, char* password, char* topic, int publish_interval, EasyWifiManager* p_easy_wifi_manager);
        void set_server();
        void reconnect();
        bool publish(String message);
        bool publish(char* message);
        bool connected();
        bool loop();

    private:
        char* convert_string_to_char_array(String str);

        char m_server[40];
        int m_port;
        char m_user[40];
        char m_password[70];
        char m_topic[255];
        WiFiClient m_esp_client;
        PubSubClient* m_p_client;
        EasyWifiManager* m_p_easy_wifi_manager;
        int publish_interval;

};

#endif