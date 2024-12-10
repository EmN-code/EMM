#include <string.h>
#include <stdio.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include "xtimer.h"

#define EMCUTE_PRIO           (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS             (16U)
#define TOPIC_MAXLEN          (64U)
#define PUBLICATION_INTERVAL  (5U * 1000000U)  // Publish every 5 seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topic_name[TOPIC_MAXLEN];

// Function to publish state
static void publish(void) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    t.name = topic_name;

    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("error: unable to register topic
");
        return;
    }

    const char *state_msg = "work";
    if (emcute_pub(&t, state_msg, strlen(state_msg), flags) != EMCUTE_OK) {
        printf("error: unable to publish data
");
        return;
    }

    printf("Published %s to topic %s
", state_msg, t.name);
}

int main(void) {
    // Initialize Emcute
    memset(subscriptions, 0, sizeof(subscriptions));
    emcute_run(EMCUTE_PRIO, stack, sizeof(stack), queue, ARRAY_SIZE(queue));

    sock_udp_ep_t remote = { .family = AF_INET6, .port = 1883 }; // MQTT-SN port

    if (inet_pton(AF_INET6, "47.102.103.1", &remote.addr.ipv6) != 1) {
        puts("error: unable to parse gateway address
");
        return 1;
    }

    strcpy(topic_name, "state_topic");

    // Connect to the broker
    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to [47.102.103.1]:1883
");
        return 1;
    }

    printf("Successfully connected to gateway at [47.102.103.1]:1883
");

    while (1) {
        publish();
        xtimer_sleep(PUBLICATION_INTERVAL / 1000000U);
    }

    return 0;
}