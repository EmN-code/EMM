#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_PORT           (1883U)
#define NUMOFSUBS             (16U)
#define TOPIC_MAXLEN          (64U)
#define STATE_MSG_INTERVAL    (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static const char *state_topic = "device/state";
static const char *state_msg_work = "work";
static const char *state_msg_not_work = "not work";
static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

static void *emcute_thread(void *arg) {
    (void) arg;
    emcute_run(EMCUTE_PORT, queue, ARRAY_SIZE(queue), stack, sizeof(stack));
    return NULL;
}

static int publish_state(const char *state) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_1;

    /* Step 1: prepare the topic */
    t.name = state_topic;

    /* Step 2: publish data */
    if (emcute_pub(&t, state, strlen(state), flags) != EMCUTE_OK) {
        printf("error: unable to publish to topic '%s'\n", t.name);
        return 1;
    }
    printf("[Pub] topic: %s, message: %s\n", state_topic, state);

    return 0;
}

static int connect_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };

    /* parse broker address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, "47.102.103.1") == NULL) {
        printf("error: unable to parse address\n");
        return 1;
    }

    /* connect to broker */
    if (emcute_con(&gw, true, state_topic, state_msg_work, strlen(state_msg_work), 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%u\n", "47.102.103.1", EMCUTE_PORT);
        return 1;
    }
    printf("Successfully connected to gateway at [%s]:%u\n", "47.102.103.1", EMCUTE_PORT);

    return 0;
}

int main(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    if (connect_broker() != 0) {
        printf("Unable to connect to the broker, exiting\n");
        return 1;
    }

    while (1) {
        publish_state(state_msg_work);
        xtimer_sleep(STATE_MSG_INTERVAL);
        
        publish_state(state_msg_not_work);
        xtimer_sleep(STATE_MSG_INTERVAL);
    }

    return 0;
}