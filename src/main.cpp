#include <Arduino.h>
#include <ArduinoJson.h>

#include "nano_esp_mqtt.h"
#include "secrets.h"
#include "time.h"

bool success = true;

int message_num = 10;

AWS_Credentials credentials{AWS_ROOT_CA1, AWS_CERT, AWS_PRIVATE_KEY, AWS_IOT_ENDPOINT, AWS_IOT_MQTT_PORT, AWS_THING_ID};

void receivedMessage(const char *message);

void setup() {
    Serial.begin(115200);
    while (!Serial)
        ;
    delay(1000);

    success = connectToWiFi(WIFI_SSID, WIFI_PWD) && setTime() && connectToMQTT_Broker(credentials) &&
              subscribe("test_sub_topic", receivedMessage);
}

void loop() {
    checkIncoming();
    if (millis() % 10000 == 0) {
        char message[100];
        Serial.println("sending test message");
        JsonDocument doc;
        doc["sensor_id"] = AWS_THING_ID;
        doc["timestamp"] = getTime();
        doc["temperature"] = message_num++;
        serializeJson(doc, message);
        sendMessage("test_topic", message);
    }
}

void receivedMessage(const char *message) {
    JsonDocument doc;
    deserializeJson(doc, message);
    Serial.println((const char *)doc["message"]);
}


