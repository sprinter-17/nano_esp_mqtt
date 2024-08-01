#include <Arduino.h>

#include "nano_esp_mqtt.h"
#include "secrets.h"

bool success = true;

int message_num = 10;

AWS_Credentials credentials{AWS_ROOT_CA1, AWS_CERT, AWS_PRIVATE_KEY, AWS_IOT_ENDPOINT, AWS_IOT_MQTT_PORT, AWS_THING_ID};

void receivedMessage(const char *message) {
    Serial.println(message);
}

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
        char message[50];
        Serial.println("sending test message");
        sprintf(message, "{\"sensor_id\": \"%s\", \"message_num\": %d}", AWS_THING_ID, message_num++);
        sendMessage("test_topic", message);
    }
}
