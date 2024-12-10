#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include <stdio.h>
#include <stdlib.h>

#define EMCUTE_PORT       (1883U)
#define SERVER_ADDR       "47.102.103.1"
#define STATE_MSG_WORK    "work"
#define STATE_MSG_NOTWORK "not work"
#define STATE_INTERVAL    (5U)

static emcute_topic_t state_topic;
static sock_udp_ep_t gw;

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("Received publication on topic %s [%i]: %.*s\n",
           topic->name, topic->id, (int)len, (char *)data);
}

static int setup_connection(void) {
    /* parse gateway address */
    if (inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4) != 1) {
        puts("Error parsing gateway address");
        return 1;
    }
    gw.port = EMCUTE_PORT;

    /* connect to the broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to the broker");
        return 1;
    }

    /* register a topic */
    state_topic.name = "device/state";
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }
    return 0;
}

int main(void) {
    puts("Starting RIOT MQTT-SN publisher application");

    while (setup_connection()) {
        xtimer_sleep(STATE_INTERVAL);
    }

    while (1) {
        /* publish the work state every STATE_INTERVAL seconds */
        const char *state = (rand() % 2) == 0 ? STATE_MSG_WORK : STATE_MSG_NOTWORK;
        if (emcute_pub(&state_topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish state");
        } else {
            printf("State published: %s\n", state);
        }
        xtimer_sleep(STATE_INTERVAL);
    }

    emcute_discon();
    return 0;
}