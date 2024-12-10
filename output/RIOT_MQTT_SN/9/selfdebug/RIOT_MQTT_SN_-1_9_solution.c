#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/sock/udp.h"
#define EMCUTE_ID         "device001"
#define EMCUTE_PRIO       (THREAD_PRIORITY_MAIN - 1)
#define STATE_TOPIC_NAME  "device/state"
#define SERVER_ADDR       "47.102.103.1"
#define SERVER_PORT       1883

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t state_topic;
static msg_t queue[8];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, stack, sizeof(stack), EMCUTE_PRIO);
    return NULL;
}

static int publish_state(const char *state) {
    unsigned flags = EMCUTE_QOS_0;
    state_topic.name = STATE_TOPIC_NAME;

    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    if (emcute_pub(&state_topic, state, strlen(state), flags) != EMCUTE_OK) {
        puts("Error: unable to publish data");
        return 1;
    }

    return 0;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = SERVER_PORT
    };
    if (inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4) != 1) {
        puts("Error: unable to parse server address");
        return 1;
    }

    memset(stack, 0, sizeof(stack));
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return 1;
    }

    while (1) {
        publish_state("work");
        xtimer_sleep(5);
        publish_state("not work");
        xtimer_sleep(5);
    }

    return 0;
}