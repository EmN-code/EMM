
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEP_ALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN false
#define MQTT_CLEAN_SESSION true

static sock_udp_t sock;
static sock_udp_ep_t server_ep = SOCK_IPV4_EP_ANY;

int mqtt_read(Network* network, unsigned char* buffer, int len, int timeout) {
    sock_udp_ep_t remote;
    ssize_t res = sock_udp_recv(&sock, buffer, len, timeout, &remote);
    if (res < 0) {
        return -1;
    }
    return res;
}

int mqtt_write(Network* network, unsigned char* buffer, int len, int timeout) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = HTONS(MQTT_BROKER_PORT) };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);
    ssize_t res = sock_udp_send(&sock, buffer, len, &remote);
    if (res < 0) {
        return -1;
    }
    return res;
}

void mqtt_network_init(Network* network) {
    network->mqttread = mqtt_read;
    network->mqttwrite = mqtt_write;
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[256];
    unsigned char readbuf[256];
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    MQTTMessage message;

    // Initialize UDP socket
    sock_udp_create(&sock, NULL, &server_ep, 0);

    // Initialize MQTT network
    mqtt_network_init(&network);

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Set MQTT connection options
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEP_ALIVE;
    options.cleansession = MQTT_CLEAN_SESSION;

    // Connect to MQTT broker
    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    // Publish state every 5 seconds
    while (1) {
        message.qos = MQTT_QOS;
        message.retained = MQTT_RETAIN;
        message.dup = 0;
        message.payload = (void*)"work";
        message.payloadlen = 4;

        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        }

        xtimer_sleep(5);
    }

    return 0;
}
