#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/tcp.h"
#include "net/sock/udp.h"
#include "net/mqttsn.h"
#include "paho_mqtt.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_MESSAGE "work or not work"
#define MQTT_CLIENT_ID "esp32"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];
static const char *state_msg = MQTT_MESSAGE;

static int on_message_received(MessageData *data) {
    printf("Message arrived on topic %.*s: %.*s\n",
            (int)data->topicName->lenstring.len, data->topicName->lenstring.data,
            (int)data->message->payloadlen, data->message->payload);
    return SUCCESS;
}

int main(void) {
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    int rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return 1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *) state_msg;
        message.payloadlen = strlen(state_msg);

        rc = MQTTPublish(&client, MQTT_TOPIC, &message);
        if (rc != SUCCESS) {
            printf("Failed to publish message, return code %d\n", rc);
        }

        xtimer_sleep(5);
    }

    return 0;
}