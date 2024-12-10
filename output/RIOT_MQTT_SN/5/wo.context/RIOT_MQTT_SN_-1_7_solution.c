#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"

#define EMCUTE_ID            "esp"
#define EMCUTE_PORT          1883  
#define EMCUTE_PRIO          (THREAD_PRIORITY_MAIN - 1)

#define STATE_TOPIC

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_sub_t subscriptions[1];
static char topic_queue[1][64U];

static const char *state[2] = { "work", "not work" };

static sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };

static void *thread_entry(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

static int publish_state(int state_index) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    t.name = "esp/state";
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to register topic");
        return 1;
    }
    if (emcute_pub(&t, state[state_index], strlen(state[state_index]), flags) != EMCUTE_OK) {
        puts("error: unable to publish data");
        return 1;
    }
    printf("success: published %s on topic %s\n", state[state_index], t.name);
    return 0;
}

int main(void) {
    msg_init_queue(topic_queue, ARRAY_SIZE(subscriptions));

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  thread_entry, NULL, "emcute");

    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, "fdaa:bbbb:bbbb:1::1");

    int state_index = 0;
    while (1) {
        publish_state(state_index);
        state_index = (state_index + 1) % 2;
        xtimer_sleep(5);
    }
    return 0;
}
