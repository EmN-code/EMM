#include <string.h>
#include <stdio.h>
#include "net/sock/udp.h"
#include "msg.h"
#include "xtimer.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "RIOT_ESP32"

static sock_udp_ep_t broker_ep = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF, .port = MQTT_BROKER_PORT };

void on_message_received(MessageData *data) {
    printf("Message arrived on topic %.*s: %.*s\n",
           data->topicName->lenstring.len, data->topicName->lenstring.data,
           data->message->payloadlen, (char *)data->message->payload);
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTStartTask(&network);

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    while (1) {
        int rc = MQTTConnect(&client, &connectData);
        if (rc != SUCCESS) {
            printf("Failed to connect, return code %d\n", rc);
        } else {
            printf("Connected to broker\n");

            char message_content[20];
            snprintf(message_content, sizeof(message_content), "working");

            MQTTMessage message;
            message.qos = QOS0;
            message.retained = 0;
            message.dup = 0;
            message.payload = (void *)message_content;
            message.payloadlen = strlen(message_content);

            rc = MQTTPublish(&client, MQTT_TOPIC, &message);
            if (rc != SUCCESS) {
                printf("Error publishing message %d\n", rc);
            } else {
                printf("Message published: %s\n", message_content);
            }
        }

        xtimer_sleep(5);
    }

    return 0;
}