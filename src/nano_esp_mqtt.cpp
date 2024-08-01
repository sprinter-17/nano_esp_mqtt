#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

#include "nano_esp_mqtt.h"

const u_long TIMEOUT = 10000;

WiFiClientSecure secureClient;
PubSubClient mqttClient(secureClient);

bool connectToWiFi(const char *ssid, const char *pwd) {
    Serial.print(F("Connecting to WiFi AP "));
    Serial.print(ssid);

    u_long start = millis();
    WiFi.begin(ssid, pwd);
    while (WiFi.status() != WL_CONNECTED && millis() < start + TIMEOUT) {
        Serial.print(".");
        delay(500);
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.print(F("Connected to "));
        Serial.println(WiFi.SSID());
        return true;
    } else {
        Serial.println(F("Timeout"));
        return false;
    }
}

bool setTime() {
    if (WiFi.status() != WL_CONNECTED)
        return false;
    Serial.print(F("Setting time using SNTP "));
    configTime(10 * 60 * 60, 0, "pool.ntp.org", "time.nist.gov");
    time_t now = time(nullptr);
    while (now < 1510592825) {
        delay(500);
        Serial.print(".");
        time(&now);
    }
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.println();
    Serial.print(F("Current GMT: "));
    Serial.print(asctime(&timeinfo));
    return true;
}

bool connectToMQTT_Broker(AWS_Credentials credentials) {
    // Configure WiFiClientSecure to use the AWS IoT device credentials
    secureClient.setCACert(credentials.root_cert);
    secureClient.setCertificate(credentials.device_cert);
    secureClient.setPrivateKey(credentials.device_key);

    // Configure mqttClient to connect to AWS MQTT broker
    mqttClient.setServer(credentials.mqtt_broker_url, credentials.mqtt_broker_port);

    // Connecting to broker
    Serial.print(F("Connecting to AWS MQTT Broker "));

    if (!mqttClient.connect(credentials.thing_id)) {
        Serial.println(F("Failed to connect"));
        return false;
    }

    u_long start = millis();
    while (!mqttClient.connected() && millis() < start + TIMEOUT) {
        Serial.print(".");
        delay(500);
    }

    if (mqttClient.connected()) {
        Serial.println();
        Serial.println(F("Connected"));
        return true;
    } else {
        Serial.println(F("Timeout"));
        return false;
    }    
}

bool sendMessage(const char* topic, const char* message) {
    if (!mqttClient.connected())
        return false;

    return mqttClient.publish(topic, message);
}