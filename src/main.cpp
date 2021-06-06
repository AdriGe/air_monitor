#include <FS.h>
#include <Arduino.h>

#include <config/config.h>

#include <sensors/AllSensors.h>
#include <sensors/SensorPMSA003.h>
#include <sensors/SensorSGP30.h>
#include <sensors/SensorBME280.h>

#include <network/EasyWifiManager.h>
#include <network/EasyMqttManager.h>
#include <network/EasyWebServer.h>

#include <displays/LedStrip.h>
#include <displays/Oled.h>


SensorPMSA003 pms(PMS_PIN_TX, PMS_PIN_RX, SENSOR_UPDATE_INTERVAL);
SensorSGP30 sgp(SENSOR_UPDATE_INTERVAL);
SensorBME280 bme(BME_I2C_ADDRESS, SENSOR_UPDATE_INTERVAL);
AllSensors all_sensors(&pms, &sgp, &bme, MEAN_NB_POINTS);

EasyWifiManager easy_wifi_manager;
EasyMqttManager* easy_mqtt_manager;
EasyWebServer easy_web_server(80, &easy_wifi_manager, &all_sensors);

Oled oled;
LedStrip led_strip;



void setup() {
    Serial.begin(115200);

    // Avoid inconsistent strip led color during boot
    led_strip.set_color(CRGB::Blue);

    // Sensors
    all_sensors.init();

    // Wifi (AP)
    easy_wifi_manager.init();

    // API
    easy_web_server.init();

    // MQTT
    char* mqtt_server = easy_wifi_manager.get_mqtt_server();
    int mqtt_port = atoi(easy_wifi_manager.get_mqtt_port());
    char* mqtt_user = easy_wifi_manager.get_mqtt_user();
    char* mqtt_password = easy_wifi_manager.get_mqtt_password();
    char* mqtt_topic = easy_wifi_manager.get_mqtt_topic();

    easy_mqtt_manager = new EasyMqttManager(mqtt_server, mqtt_port, mqtt_user, mqtt_password, mqtt_topic, 5000, &easy_wifi_manager);

    // Oled screen
    oled.init();
}


unsigned long mqtt_last_publish_time = 0;

void loop() {
    // Handle API requests
    easy_web_server.handle_client();

    // Read sensors values
    bool is_new_sensor_values = all_sensors.read();

    // Handle Oled display
    oled.loop(&all_sensors, 2000, 2000, 500, 200, 2000, 3000, 500, 50);

    // Handle Leds strip
    led_strip.loop(&all_sensors);

    // Handle MQTT
    easy_mqtt_manager->loop();

    if(millis() - mqtt_last_publish_time > MQTT_PUBLISH_INTERVAL)
    {
        //Publish to MQTT Topic
        String sensors_values = all_sensors.get_json_object();
        easy_mqtt_manager->publish(sensors_values);

        mqtt_last_publish_time = millis();
    }
}