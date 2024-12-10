#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>
#include <thread.h>
#include <xtimer.h> // for xtimer_sleep() and others

#define EMCUTE_ID           "my_device"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define STATE_SIZE          (20U)
#define DEFAULT_TOPIC_ID    (1U)
#define DEFAULT_HOST       "47.102.103.1"
#define DEFAULT_PORT       (1883U)

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_topic_t topic;
static char state[STATE_SIZE];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(DEFAULT_PORT, EMCUTE_ID);
    return NULL;
}

static int mqtt_connect(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = DEFAULT_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, DEFAULT_HOST);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect broker.");
        return 1;
    }
    return 0;
}

int main(void) {
    memset(stack, 0, sizeof(stack));
    memset(state, 0, sizeof(state));

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    if (mqtt_connect() != 0) {
        return 1;
    }

    topic.name = "state";
    topic.id = DEFAULT_TOPIC_ID;

    while (1) {
        /* Simulating work state toggling */
        static bool work = true;
        snprintf(state, sizeof(state), "work %s", work ? "ON" : "OFF");
        work = !work;
        
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish data.");
        }
        xtimer_sleep(5);
    }

    return 0;
}