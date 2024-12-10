
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void network_init(Network *n) {
    n->mqttread = sock_tcp_read;
    n->mqttwrite = sock_tcp_write;
    n->sock = SOCK_ADDR_ANY_NETIF;
}

static int network_connect(Network *n, const char *address, int port) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = port };
    inet_pton(AF_INET, address, &remote.addr.ipv4);

    return sock_tcp_connect(&n->sock, &remote, &local, 0);
}

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    MQTTMessage message;
    char sendbuf[100], readbuf[100];

    network_init(&network);
    if (network_connect(&network, BROKER_IP, BROKER_PORT) != 0) {
        puts("Failed to connect to broker");
        return 1;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_client";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    while (1) {
        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            puts("Failed to publish message");
        } else {
            puts("Message published");
        }

        xtimer_usleep(INTERVAL);
    }

    return 0;
}
