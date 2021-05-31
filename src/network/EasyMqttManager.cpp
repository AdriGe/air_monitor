#include <network/EasyMqttManager.h>

using namespace std;

EasyMqttManager::EasyMqttManager(char* server, int port, char* user, char* password, char* topic, int publish_interval, EasyWifiManager* p_easy_wifi_manager) 
{
    strcpy(m_server, server);
    m_port = port;
    strcpy(m_user, user);
    strcpy(m_password, password);
    strcpy(m_topic, topic);
    m_p_client = new PubSubClient(m_esp_client);
    m_p_easy_wifi_manager = p_easy_wifi_manager;

    set_server();
}

void EasyMqttManager::set_server(){
    Serial.println("[EasyMqttManager] Setting server");
    m_p_client->setServer(m_server, m_port);
}

void EasyMqttManager::reconnect() {
    int mqtt_try_count = 0;
    int max_mqtt_try_count = 3;
    // Loop until we're reconnected
    while (!m_p_client->connected()) {
        Serial.println("[EasyMqttManager] Attempting MQTT connection...");
        
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("[EasyMqttManager] WiFi.status() != WL_CONNECTED");
            Serial.println("[EasyMqttManager] Reconnecting to WiFi...");
            WiFi.begin(m_p_easy_wifi_manager->get_ssid(), m_p_easy_wifi_manager->get_psk());
        }

        // Attempt to connect
        if (m_p_client->connect("ESP8266Client", m_user, m_password)) {
            Serial.println("[EasyMqttManager] Connected");
        } else {
            if(mqtt_try_count>=max_mqtt_try_count){
                Serial.println("[EasyMqttManager] Max try reached while connecting to MQTT server. Going into AP Mode !");
                WiFiManager wifiManager;
                wifiManager.autoConnect();
            } else {
                mqtt_try_count++;
                Serial.print("[EasyMqttManager] Failed, rc=");
                Serial.println(m_p_client->state());
                Serial.println("[EasyMqttManager] Try again in 5 seconds");
                delay(5000);
            }
            
        }
    }
}

char* EasyMqttManager::convert_string_to_char_array(String str) {
    int n = str.length();
    char *message_char_array = new char[n+1+10];
    strcpy(message_char_array, str.c_str());

    return message_char_array;
}

bool EasyMqttManager::publish(char* message) {
    if (!connected()) {
        reconnect();
    }

    loop();
    Serial.print("[EasyMqttManager] Publishing to ");
    Serial.println(m_topic);
    Serial.println(message);

    return m_p_client->publish(m_topic, message);
}

bool EasyMqttManager::publish(String message) {    
    char* message_char_array = convert_string_to_char_array(message);
    
    bool publish_response = publish(message_char_array);

    delete[] message_char_array; //Don't forget to delete the char array or a memory leak will happen.

    return publish_response;
}

bool EasyMqttManager::connected() {
    return m_p_client->connected();
}

bool EasyMqttManager::loop() {
    return m_p_client->loop();
}