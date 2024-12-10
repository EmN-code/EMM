
#include <stdio.h>
#include <string.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <xtimer.h>
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000 * 1000U)  // 5 seconds in microseconds

static void network_init(Network* network) {
    network->mqttread = NULL;
    network->mqttwrite = NULL;
}

static int network_connect(Network* network, const char* host, int port) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = port };
    inet_pton(AF_INET, host, &remote.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        return -1;
    }

    network->sock = sock;
    return 0;
}

static int network_write(Network* network, unsigned char* buf, int len, int timeout) {
    return sock_udp_send(network->sock, buf, len, NULL);
}

static int network_read(Network* network, unsigned char* buf, int len, int timeout) {
    return sock_udp_recv(network->sock, buf, len, timeout, NULL);
}

static void network_disconnect(Network* network) {
    sock_udp_close(network->sock);
}

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;
    char buf[100];

    network_init(&network);
    if (network_connect(&network, SERVER_IP, SERVER_PORT) != 0) {
        printf("Failed to connect to server\n");
        return 1;
    }

    MQTTClientInit(&client, &network, 1000, buf, sizeof(buf), buf, sizeof(buf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_MQTT_CLIENT";

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        network_disconnect(&network);
        return 1;
    }

    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    while (1) {
        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        }
        xtimer_usleep(INTERVAL);
    }

    network_disconnect(&network);
    return 0;
}
