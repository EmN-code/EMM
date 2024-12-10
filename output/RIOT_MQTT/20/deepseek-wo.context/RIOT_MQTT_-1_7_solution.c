
#include <stdio.h>
#include <string.h>
#include "net/sock/tcp.h"
#include "xtimer.h"
#include "net/mqtt.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void network_connect(Network *n, const char *address, int port) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;
    sock_tcp_t sock;

    remote.family = AF_INET;
    remote.port = port;
    inet_pton(AF_INET, address, &remote.addr.ipv4);

    sock_tcp_create(&sock, &local, &remote, 0);
    sock_tcp_connect(&sock, &remote, SOCK_NO_TIMEOUT);

    n->sock = sock;
    n->mqttread = (int (*)(struct Network*, unsigned char*, int, int))sock_tcp_read;
    n->mqttwrite = (int (*)(struct Network*, unsigned char*, int, int))sock_tcp_write;
}

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;
    char sendbuf[100], readbuf[100];

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "riot_client";
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;

    network_connect(&network, BROKER_IP, BROKER_PORT);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    while (1) {
        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        }
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
