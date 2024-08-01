#ifndef NANO_ESP_MQTT_H
#define NANO_ESP_MQTT_H

#include <Arduino.h>

typedef struct {
    const char* root_cert;
    const char* device_cert;
    const char* device_key;
    const char* mqtt_broker_url;
    const u_int16_t mqtt_broker_port;
    const char* thing_id;
} AWS_Credentials;

bool setTime();
bool connectToWiFi(const char* ssid, const char* pwd);
bool connectToMQTT_Broker(AWS_Credentials credentials);
bool sendMessage(const char* topic, const char* message);

#endif