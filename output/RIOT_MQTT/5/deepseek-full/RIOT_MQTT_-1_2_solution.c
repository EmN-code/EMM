
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/sock/util.h"
#include "net/mqttsn.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define MESSAGE "work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

void init_udp_socket(void) {
    remote.family = AF_INET;
    remote.port = BROKER_PORT;
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }
}

void publish_message(void) {
    char buf[64];
    snprintf(buf, sizeof(buf), "PUBLISH %s %s", TOPIC, MESSAGE);

    if (sock_udp_send(&sock, buf, strlen(buf), &remote) < 0) {
        puts("Error sending message");
    } else {
        puts("Message sent");
    }
}

int main(void) {
    init_udp_socket();

    while (1) {
        publish_message();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
