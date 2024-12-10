
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <timex.h>
#include <xtimer.h>
#include <unistd.h>
#include <stdbool.h>
#include <msg.h>
#include <thread.h>
#include <mbox.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_TIMEOUT 10000L

#define BUFFER_SIZE 100
#define SEND_BUF_SIZE 256
#define READ_BUF_SIZE 256

static Network network;
static MQTTClient mqttClient;
static unsigned char sendbuf[SEND_BUF_SIZE];
static unsigned char readbuf[READ_BUF_SIZE];

void messageArrived(MessageData* data) {
    printf("Message arrived on topic %.*s: %.*s\n",
           data->topicName->lenstring.len, data->topicName->lenstring.data,
           data->message->payloadlen, data->message->payload);
}

int main(void) {
    msg_t msg_queue[16];
    msg_init_queue(msg_queue, 16);

    NetworkInit(&network);
    MQTTClientInit(&mqttClient, &network, MQTT_TIMEOUT, sendbuf, SEND_BUF_SIZE, readbuf, READ_BUF_SIZE);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    int rc = MQTTConnect(&mqttClient, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return -1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        MQTTMessage message;
        message.qos = MQTT_QOS;
        message.retained = 0;
        message.payload = (void*)MQTT_MESSAGE;
        message.payloadlen = strlen(MQTT_MESSAGE);

        rc = MQTTPublish(&mqttClient, MQTT_TOPIC, &message);
        if (rc != 0) {
            printf("Failed to publish message, return code %d\n", rc);
        } else {
            printf("Message published\n");
        }

        xtimer_sleep(5);
    }

    return 0;
}
