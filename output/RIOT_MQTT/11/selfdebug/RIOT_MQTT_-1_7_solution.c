#include <stdio.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "mqtt.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define INTERVAL (5U * US_PER_SEC) // 5 seconds

static sock_udp_ep_t broker;

void setup_network(Network *network) {
    // Initialize your network settings here
    NetworkInit(network);
    NetworkConnect(network, BROKER_IP, BROKER_PORT);
}

int main(void) {
    static const char *topic = "device/state";
    static const char *message_work = "work";
    static const char *message_not_work = "not work";
    Network network;
    MQTTClient client;

    // Initialize network and MQTT client
    setup_network(&network);
    unsigned char send_buffer[100];
    unsigned char read_buffer[100];
    MQTTClientInit(&client, &network, 1000, send_buffer, sizeof(send_buffer), read_buffer, sizeof(read_buffer));

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &data) != SUCCESS) {
        printf("MQTT Connection failed\n");
        return -1;
    }

    printf("MQTT Connected\n");

    // Publishing loop
    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)message_work;
        message.payloadlen = strlen(message_work);

        if (MQTTPublish(&client, topic, &message) != SUCCESS) {
            printf("Error publishing message\n");
        } else {
            printf("Message published: %s\n", message_work);
        }

        xtimer_sleep(INTERVAL / US_PER_SEC);

        // Alternate message for test purpose
        message.payload = (void *)message_not_work;
        message.payloadlen = strlen(message_not_work);

        if (MQTTPublish(&client, topic, &message) != SUCCESS) {
            printf("Error publishing message\n");
        } else {
            printf("Message published: %s\n", message_not_work);
        }

        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}