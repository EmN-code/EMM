#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_ID          "mqttsn-client"
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define BROKER_PORT        1883
#define BROKER_ADDRESS     "47.102.103.1"

#define STATE_TOPIC        "device/state"
#define PUBLISH_INTERVAL   (5U) * US_PER_SEC

static char status_stack[THREAD_STACKSIZE_DEFAULT];
static msg_t status_queue[8];

static emcute_sub_t subscriptions[1];
static char stack[THREAD_STACKSIZE_DEFAULT];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(BROKER_PORT, status_queue, sizeof(status_queue));
    return NULL;
}

static int publish_state(void) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    const char *state_message = "work or not work";

    /* setup topic */
    t.name = STATE_TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("error: unable to register topic
");
        return 1;
    }

    /* publish data */
    if (emcute_pub(&t, state_message, strlen(state_message), flags) != EMCUTE_OK) {
        printf("error: unable to publish data
");
        return 1;
    }

    printf("Published %s on topic %s
", state_message, STATE_TOPIC);

    return 0;
}

int main(void) {
    printf("Starting MQTT-SN emcute example...
");

    /* Start emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* Start periodic publishing */
    while (1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    /* This should never be reached */
    return 0;
}