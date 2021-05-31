#include <network/EasyWifiManager.h>

using namespace std;

EasyWifiManager::EasyWifiManager(){
    // Default MQTT values for WifiManager configuration portal
    strcpy(mqtt_server, DEFAULT_MQTT_SERVER);
    strcpy(mqtt_port, DEFAULT_MQTT_PORT);
    strcpy(mqtt_user, DEFAULT_MQTT_USER);
    strcpy(mqtt_password, DEFAULT_MQTT_PASSWORD);
    strcpy(mqtt_topic, DEFAULT_MQTT_TOPIC);

}

void EasyWifiManager::init() {
    load_config();
    
    WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
    WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6);
    WiFiManagerParameter custom_mqtt_user("user", "mqtt user", mqtt_user, 40);
    WiFiManagerParameter custom_mqtt_password("password", "mqtt password", mqtt_password, 70);
    WiFiManagerParameter custom_mqtt_topic("topic", "mqtt topic", mqtt_topic, 255);

    // WiFiManager
    // Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    
    //add all your parameters here
    wifiManager.addParameter(&custom_mqtt_server);
    wifiManager.addParameter(&custom_mqtt_port);
    wifiManager.addParameter(&custom_mqtt_user);
    wifiManager.addParameter(&custom_mqtt_password);
    wifiManager.addParameter(&custom_mqtt_topic);
    

    // fetches ssid and pass from eeprom and tries to connect
    // if it does not connect it starts an access point with the name ESP + ChipID
    wifiManager.autoConnect();
    
    // if you get here you have connected to the WiFi
    Serial.println("[EasyWifiManager] Connected To WiFi.");
    m_ssid = WiFi.SSID();
    m_psk = WiFi.psk();
    
    
    strcpy(mqtt_server, custom_mqtt_server.getValue());
    strcpy(mqtt_port, custom_mqtt_port.getValue());
    strcpy(mqtt_user, custom_mqtt_user.getValue());
    strcpy(mqtt_password, custom_mqtt_password.getValue());
    strcpy(mqtt_topic, custom_mqtt_topic.getValue());

    save_config();
}


void EasyWifiManager::load_config() {
    if (SPIFFS.begin()) {
        Serial.println("[EasyWifiManager] Mounted file system");

        if (SPIFFS.exists("/config.json")) {
            //File exists, reading and loading
            Serial.println("[EasyWifiManager] Reading config file");
            File configFile = SPIFFS.open("/config.json", "r");

            if (configFile) {
                Serial.println("[EasyWifiManager] Opened config file");
                size_t size = configFile.size();
                // Allocate a buffer to store contents of the file.
                std::unique_ptr<char[]> buf(new char[size]);

                configFile.readBytes(buf.get(), size);
                DynamicJsonBuffer jsonBuffer;
                JsonObject& json = jsonBuffer.parseObject(buf.get());
                json.printTo(Serial);
                
                if (json.success()) {
                    Serial.println("[EasyWifiManager] Parsed json");
                    strcpy(mqtt_server, json["mqtt_server"]);
                    strcpy(mqtt_port, json["mqtt_port"]);
                    strcpy(mqtt_user, json["mqtt_user"]);
                    strcpy(mqtt_password, json["mqtt_password"]);
                    strcpy(mqtt_topic, json["mqtt_topic"]);
                } else {
                    Serial.println("[EasyWifiManager] Failed to load json config");
                }
            }
        }
    } else {
        Serial.println("[EasyWifiManager] Failed to mount FS");
    }
}

void EasyWifiManager::save_config(){
    
    //save the custom parameters to FS
    Serial.println("[EasyWifiManager] Saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["mqtt_server"] = mqtt_server;
    json["mqtt_port"] = mqtt_port;
    json["mqtt_user"] = mqtt_user;
    json["mqtt_password"] = mqtt_password;
    json["mqtt_topic"] = mqtt_topic;
        

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
        Serial.println("[EasyWifiManager] Failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
}


void EasyWifiManager::reset_config() {
    WiFiManager wifiManager;
    wifiManager.resetSettings();
}

String EasyWifiManager::get_ssid() { return m_ssid; }
String EasyWifiManager::get_psk() { return m_psk; }
char* EasyWifiManager::get_mqtt_server() { return mqtt_server; }
char* EasyWifiManager::get_mqtt_port() { return mqtt_port; }
char* EasyWifiManager::get_mqtt_user() { return mqtt_user; }
char* EasyWifiManager::get_mqtt_password() { return mqtt_password; }
char* EasyWifiManager::get_mqtt_topic() { return mqtt_topic; }


