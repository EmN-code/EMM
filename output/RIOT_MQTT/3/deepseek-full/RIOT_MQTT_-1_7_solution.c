
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <ztimer.h>
#include <msg.h>
#include <thread.h>
#include <mutex.h>
#include <timex.h>
#include <kernel_defines.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "riot_esp32"
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_KEEPALIVE 60

typedef struct {
    sock_udp_t sock;
    sock_udp_ep_t local;
    sock_udp_ep_t remote;
} Network;

void NetworkInit(Network *n) {
    memset(&n->local, 0, sizeof(n->local));
    memset(&n->remote, 0, sizeof(n->remote));
    n->local.port = 0;
    n->local.family = AF_INET;
    n->remote.port = htons(MQTT_BROKER_PORT);
    n->remote.family = AF_INET;
    inet_pton(AF_INET, MQTT_BROKER_IP, &n->remote.addr.ipv4);
    sock_udp_create(&n->sock, &n->local, &n->remote, 0);
}

int NetworkConnect(Network *n) {
    return sock_udp_connect(&n->sock, &n->remote);
}

void publish_message(Network *n) {
    const char *message = MQTT_MESSAGE;
    sock_udp_send(&n->sock, message, strlen(message), &n->remote);
}

int main(void) {
    Network network;
    NetworkInit(&network);
    if (NetworkConnect(&network) < 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        publish_message(&network);
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}
