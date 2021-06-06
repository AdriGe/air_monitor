#ifndef DEF_AIR_MONITOR_CONFIGURATION
#define DEF_AIR_MONITOR_CONFIGURATION

//All Sensors
#define MEAN_NB_POINTS 10
#define SENSOR_UPDATE_INTERVAL 5000

//PMS
#define PMS_PIN_TX 14
#define PMS_PIN_RX 12
#define PMS_READ_INTERVAL 2000
#define PMS_READ_DELAY 3000

//BME280
#define BME_I2C_ADDRESS 0x76

//DS18B20
#define DS18B20_GPIO_PIN 2

//Leds Strip
#define LED_STRIP_DATA_PIN 3
#define NB_LEDS 10

//MQTT
#define MQTT_PUBLISH_INTERVAL 2000
#define DEFAULT_MQTT_SERVER "10.0.0.89"
#define DEFAULT_MQTT_PORT "1883"
#define DEFAULT_MQTT_USER "mqttUsername"
#define DEFAULT_MQTT_PASSWORD "mqttPassword"
#define DEFAULT_MQTT_TOPIC "sensor/air_monitor"

#endif