#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "paho_mqtt/MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "device/status"
#define MESSAGE "work or not work"
#define MS_DEALY (5000)

Network network;
MQTTClient client;
unsigned char sendbuf[100], readbuf[100];

void setup_mqtt(MQTTClient *c) {
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    MQTTClientInit(c, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_mqtt_client";

    while (1) {
        int rc = MQTTConnect(c, &connectData);
        if (rc != 0) {
            printf("MQTT Connection failed: %d\n", rc);
            xtimer_sleep(1);
            continue;
        }
        printf("MQTT connected\n");
        break;
    }
}

int main(void) {
    printf("Starting ESP32 MQTT example...\n");

    while (1) {
        int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed: %d\n", rc);
            xtimer_sleep(1);
            continue;
        }
        printf("Network connected\n");
        break;
    }

    setup_mqtt(&client);

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)MESSAGE;
        message.payloadlen = strlen(MESSAGE);

        int rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != 0) {
            printf("Publish failed: %d\n", rc);
            xtimer_sleep(1);
            continue;
        }
        printf("Published: %s\n", MESSAGE);
        
        xtimer_sleep(MS_DEALY / 1000);
    }

    return 0;
}