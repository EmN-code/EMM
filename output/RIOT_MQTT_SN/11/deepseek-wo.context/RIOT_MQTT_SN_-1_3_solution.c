
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "emcute.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"

static sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
static emcute_topic_t topic;

void publish_state(void) {
    static const char *states[] = { "work", "not work" };
    static uint8_t state_index = 0;
    const char *state = states[state_index];
    state_index = (state_index + 1) % 2;

    if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
    } else {
        printf("Published: %s\n", state);
    }
}

int main(void) {
    sock_udp_ep_t local = { .family = AF_INET, .port = 0 };
    sock_udp_t sock;

    if (inet_pton(AF_INET, SERVER_ADDR, &server.addr.ipv4) <= 0) {
        puts("Error: unable to convert server address");
        return 1;
    }

    if (sock_udp_create(&sock, &local, &server, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    if (emcute_con(&server, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
