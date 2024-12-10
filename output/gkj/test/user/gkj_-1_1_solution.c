
#include <stdio.h>
#include <string.h>
#include "MQTTClient.h"
#include "thread.h"
#include "xtimer.h"

#define MQTT_BROKER_ADDR "tcp://test.mosquitto.org:1883"
#define MQTT_CLIENT_ID "riot_mqtt_client"
#define MQTT_TOPIC "riot/test"
#define MQTT_PAYLOAD "Hello from RIOT OS!"
#define MQTT_QOS 0
#define MQTT_RETAIN 0

static MQTTClient mqtt_client;
static char mqtt_thread_stack[THREAD_STACKSIZE_DEFAULT];

void message_arrived(MessageData* data) {
    printf("Message arrived on topic %.*s: %.*s\n",
           data->topicName->lenstring.len, data->topicName->lenstring.data,
           data->message->payloadlen, (char*)data->message->payload);
}

void* mqtt_thread(void* arg) {
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.clientID.cstring = MQTT_CLIENT_ID;
    connect_data.keepAliveInterval = 20;
    connect_data.cleansession = 1;

    if (MQTTConnect(&mqtt_client, &connect_data) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    printf("Connected to MQTT broker\n");

    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    if (MQTTPublish(&mqtt_client, MQTT_TOPIC, &message) != 0) {
        printf("Failed to publish message\n");
    } else {
        printf("Message published\n");
    }

    while (1) {
        MQTTYield(&mqtt_client, 1000);
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    printf("Starting MQTT client on RIOT OS\n");

    NetworkInit(&mqtt_client.network);

    if (NetworkConnect(&mqtt_client.network, MQTT_BROKER_ADDR, 1883) != 0) {
        printf("Failed to connect to network\n");
        return 1;
    }

    printf("Network connected\n");

    mqtt_client.messageHandler = message_arrived;

    thread_create(mqtt_thread_stack, sizeof(mqtt_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
