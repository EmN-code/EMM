#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <net/mqttsn.h>
#include <net/af.h>
#include <xtimer.h>
#include "msg.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)
#define TOPIC_NAME "state"

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void *emcute_thread(void *arg) {
    (void) arg;
    emcute_run(BROKER_PORT, "esp32-client");
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) != 1) {
        puts("Error: unable to parse broker address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        const char *state = "work or not work";
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish");
        }
        xtimer_sleep(PUB_INTERVAL);
    }
    
    emcute_discon();

    return 0;
}