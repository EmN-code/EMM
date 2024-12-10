#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "timex.h"
#include "ztimer.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

#ifndef EMCUTE_ID
#define EMCUTE_ID           ("gertrud")
#endif

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

static int publish(const char *topic, const char *data) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    /* step 1: get topic id */
    t.name = topic;
    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("[failed] unable to obtain topic ID\n");
        return 1;
    }

    /* step 2: publish data */
    if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
        printf("[failed] unable to publish data\n");
        return 1;
    }

    printf("[done] published %s on topic %s\n", data, topic);
    return 0;
}

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, stack, sizeof(stack), EMCUTE_PRIO);
    return NULL;    /* should never be reached */
}

int main(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));
    memset(topics, 0, (NUMOFSUBS * TOPIC_MAXLEN));

    sock_udp_ep_t gw = { .family = AF_INET6, .port = 1883 };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, "47.102.103.1");

    if (emcute_con(&gw, true, EMCUTE_ID, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%i\n", "47.102.103.1", (int)gw.port);
        return 1;
    }

    const char *topics[] = {"device/state"};

    while (1) {
        publish(topics[0], "work");
        ztimer_sleep(ZTIMER_MSEC, 5000);
        publish(topics[0], "not work");
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}