
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <stddef.h>
#include <paho_mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work or not work"
#define MQTT_QOS 0
#define MQTT_RETAIN false

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

void mqtt_connect(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    IPStackConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID.cstring = MQTT_CLIENT_ID;
    data.keepAliveInterval = 20;
    data.cleansession = 1;
    data.MQTTVersion = 3;

    if (MQTTConnect(&client, &data) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    printf("Connected to MQTT broker\n");
}

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
        printf("Failed to publish message\n");
        return;
    }

    printf("Message published: %s\n", MQTT_MESSAGE);
}

int main(void) {
    mqtt_connect();

    while (1) {
        mqtt_publish();
        vTaskDelay(pdMS_TO_TICKS(5000)); // Delay for 5 seconds
    }

    return 0;
}
