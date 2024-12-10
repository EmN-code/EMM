#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_ID           "esp32"
#define EMCUTE_PORT         1883
#define SERVER_ADDR         "47.102.103.1"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define PUBLISH_INTERVAL    (5U * 1000000U) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;
static char *state = "work";

void* emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_run(EMCUTE_PORT, EMCUTE_PRIO, EMCUTE_ID);
    return NULL;
}

int main(void) {
    /* Initialize emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* Initialize a new emcute endpoint for the server */
    sock_udp_ep_t gw = {
        .family = AF_INET6,
        .netif = NULL,
        .port = EMCUTE_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, SERVER_ADDR);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d
", SERVER_ADDR, EMCUTE_PORT);
        return 1;
    }
    printf("Successfully connected to [%s]:%d
", SERVER_ADDR, EMCUTE_PORT);

    /* Setup the topic we want to publish to */
    topic.name = "device/state";
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic
");
        return 1;
    }

    while (1) {
        /* Publish state periodically */
        res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish data
");
        } else {
            printf("Published state: %s
", state);
        }

        /* Wait for the next publish interval */
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    /* Disconnect from gateway */
    res = emcute_discon();
    if (res != EMCUTE_OK) {
        printf("Error: unable to disconnect
");
        return 1;
    }

    return 0;
}