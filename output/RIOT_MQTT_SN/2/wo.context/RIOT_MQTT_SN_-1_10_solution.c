#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "net/sock/udp.h"
#include "net/emcute.h"

#define BROKER_PORT   1883
#define BROKER_IP     "47.102.103.1"
#define STATE_TOPIC   "state"

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t msg_queue[8];

static void *emcute_thread(void *arg) {
    emcute_sub_t subscription;
    emcute_topic_t topic;
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

    /* validate and convert broker IP address */
    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) == 0) {
        puts("Error: unable to parse broker IPv4 address.");
        return NULL;
    }

    /* Initialize emcute */
    memset(stack, 0, sizeof(stack));
    emcute_run(BROKER_PORT, "mQTT-SN");

    /* try to connect */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to broker.");
        return NULL;
    }

    /* register topic */
    topic.name = STATE_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic.");
        return NULL;
    }

    /* setup subscription */
    subscription.topic = topic;
    subscription.cb = NULL;
    if (emcute_sub(&subscription, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to subscribe.");
        return NULL;
    }

    /* publish every 5 seconds */
    while (1) {
        const char *state = "work";
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish state.");
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    /* start the emcute thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, emcute_thread, NULL, "emcute");
    return 0;
}