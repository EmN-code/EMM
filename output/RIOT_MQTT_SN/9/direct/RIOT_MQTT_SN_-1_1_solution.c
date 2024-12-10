#include "xtimer.h"
#include "net/ipv6/addr.h"
#include "net/emcute.h"
#include <stdio.h>
#include <string.h>

#define EMCUTE_PORT     1883
#define EMCUTE_ID       "esp32"
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define BROKER_IP       "47.102.103.1"
#define PUB_TOPIC       "device/state"
#define DELAY           (5U * 1000000U)  // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t t;

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, stack, sizeof(stack), EMCUTE_PRIO);
    return NULL;
}

static int publish(const char *state) {
    unsigned flags = EMCUTE_QOS_0;
    emcute_topic_t topic;
    topic.id = 0; /* dummy value */
    topic.name = PUB_TOPIC;

    if (emcute_pub(&topic, state, strlen(state), flags) != EMCUTE_OK) {
        printf("[FAILED] to publish data\n");
        return 1;
    }

    printf("Published %s on topic %s\n", state, PUB_TOPIC);
    return 0;
}

int main(void) {
    /* Start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    /* Configuration of broker address */
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP);

    if (emcute_con(&gw, true, "") != EMCUTE_OK) {
        printf("[FAILED] to connect to broker\n");
        return 1;
    }
    printf("Connected to broker at %s\n", BROKER_IP);

    /* Main loop to publish state periodically */
    while (1) {
        publish("work");
        xtimer_sleep(DELAY / 1000000);
        publish("not work");
        xtimer_sleep(DELAY / 1000000);
    }

    return 0;
}