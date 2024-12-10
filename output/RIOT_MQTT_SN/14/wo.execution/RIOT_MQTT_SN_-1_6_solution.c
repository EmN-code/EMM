#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "msg.h"
#include "esp_system.h"
#include "dht_params.h"
#include "arpa/inet.h"

#define EMCUTE_ID           "device1"
#define EMCUTE_PORT         1885
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static sock_udp_ep_t gw;

topic_t state_topic;

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL; /* should never be reached */
}

int main(void) {
    /* initialize multicast and state topic */
    memset(&gw, 0, sizeof(gw));
    gw.family = AF_INET;
    gw.netif = SOCK_ADDR_ANY_NETIF;

    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) != 1) {
        puts("error: unable to parse IPv4 address\n");
        return 1;
    }

    gw.port = EMCUTE_PORT;

    /* initialize emcute */
    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_topic_t topic;
    strcpy(topic.name, "state");

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to register topic\n");
        return 1;
    }

    state_topic = topic.id;

    /* start emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    /* connect to MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect\n");
        return 1;
    }

    while (1) {
        const char *state = (rand() % 2) ? "work" : "not work";

        if (emcute_pub(&state_topic, state, strlen(state), 0) != EMCUTE_OK) {
            puts("error: unable to publish\n");
        }

        ztimer_sleep(ZTIMER_MSEC, 5000);  // sleep for 5 seconds
    }

    return 0;
}