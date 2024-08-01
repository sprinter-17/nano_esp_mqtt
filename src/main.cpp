#include <Arduino.h>

#include "nano_esp_mqtt.h"
#include "secrets.h"

bool success = true;

int message_num = 10;

AWS_Credentials credentials {
  AWS_ROOT_CA1, AWS_CERT, AWS_PRIVATE_KEY, AWS_IOT_ENDPOINT, AWS_IOT_MQTT_PORT, AWS_THING_ID
};

void setup() {
    Serial.begin(115200);
    while (!Serial)
        ;
    delay(1000);

    success = connectToWiFi(WIFI_SSID, WIFI_PWD) && setTime() && connectToMQTT_Broker(credentials);
}

void loop() {
    char message[50];
    Serial.println("sending test message");
    sprintf(message, "{\"sensor_id\": 5, \"message_num\": %d}", message_num++);
    sendMessage("test_topic", message);
    delay(10000);
}
