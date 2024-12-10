
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

void mqtt_message_handler(MessageData* data)
{
    printf("Received message on topic %.*s: %.*s\n",
           data->topicName->lenstring.len, data->topicName->lenstring.data,
           data->message->payloadlen, data->message->payload);
}

void* mqtt_thread(void* arg)
{
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.clientID.cstring = MQTT_CLIENT_ID;
    connect_data.keepAliveInterval = 20;
    connect_data.cleansession = 1;

    printf("Connecting to MQTT broker...\n");
    int rc = MQTTConnect(&mqtt_client, &connect_data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return NULL;
    }

    printf("Subscribing to topic %s...\n", MQTT_TOPIC);
    rc = MQTTSubscribe(&mqtt_client, MQTT_TOPIC, MQTT_QOS, mqtt_message_handler);
    if (rc != 0) {
        printf("Failed to subscribe to topic, return code %d\n", rc);
        return NULL;
    }

    while (1) {
        printf("Publishing message...\n");
        MQTTMessage message;
        message.qos = MQTT_QOS;
        message.retained = MQTT_RETAIN;
        message.payload = (void*)MQTT_PAYLOAD;
        message.payloadlen = strlen(MQTT_PAYLOAD);

        rc = MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
        if (rc != 0) {
            printf("Failed to publish message, return code %d\n", rc);
        }

        xtimer_sleep(10);
    }

    return NULL;
}

int main(void)
{
    printf("Starting MQTT client on RIOT OS...\n");

    NetworkInit(&mqtt_client.network);
    mqtt_client.isConnected = 0;
    mqtt_client.commandTimeoutMs = 30000;

    thread_create(mqtt_thread_stack, sizeof(mqtt_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
