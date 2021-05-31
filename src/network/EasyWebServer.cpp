#include <network/EasyWebServer.h>

using namespace std;

EasyWebServer::EasyWebServer(uint16_t port, EasyWifiManager* p_easy_wifi_manager, AllSensors* p_all_sensors) {
    m_port = port;
    m_p_server = new ESP8266WebServer(m_port);
    m_p_easy_wifi_manager = p_easy_wifi_manager;
    m_p_all_sensors = p_all_sensors;
}


void EasyWebServer::init() {
    // https://www.esp8266.com/viewtopic.php?p=25894
    m_p_server->on("/resetWifi", std::bind(&EasyWebServer::handle_resetWifi, this));
    m_p_server->on("/getJsonValues", std::bind(&EasyWebServer::handle_getJsonValues, this));
    m_p_server->onNotFound(std::bind(&EasyWebServer::handle_NotFound, this));
    
    m_p_server->begin();
}


void EasyWebServer::handle_client() {
    m_p_server->handleClient();
}


void EasyWebServer::handle_resetWifi() {
    m_p_easy_wifi_manager->reset_config();
    m_p_server->send(200, "text/html", "{\"action\": \"resetWifi\"}");
}


void EasyWebServer::handle_getJsonValues() {
    String json_object_str = m_p_all_sensors->get_json_object();
    m_p_server->send(200, "application/json", json_object_str);
}

void EasyWebServer::handle_NotFound(){
    m_p_server->send(404, "text/plain", "Not found");
}
