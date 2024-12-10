
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ADDR "47.102.103.1"
#define TOPIC "state"
#define MSG "work"

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t t;

void publish_state(void) {
    char buf[10];
    strncpy(buf, MSG, sizeof(buf));
    if (emcute_pub(&t, buf, strlen(buf), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    }
}

int main(void) {
    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_t sock;

    if (inet_pton(AF_INET, EMCUTE_ADDR, &gw.addr.ipv4) <= 0) {
        puts("Error: unable to convert address");
        return 1;
    }

    if (sock_udp_create(&sock, &local, &gw, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    t.name = TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
