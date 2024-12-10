#include <xtimer.h>
#include <string.h>
#include <stdlib.h>

#include "net/emcute.h"

#define EMCUTE_PORT     (1883U)
#define BROKER_ADDR     "47.102.103.1"
#define PUB_INTERVAL    (5U) /* publish every 5 seconds */

static emcute_topic_t topic;

void *emcute_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    gw.addr.ipv4[0] = 47;
    gw.addr.ipv4[1] = 102;
    gw.addr.ipv4[2] = 103;
    gw.addr.ipv4[3] = 1;

    /* connect to the broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%i\n", BROKER_ADDR, EMCUTE_PORT);
        return NULL;
    }

    /* set topic */
    topic.name = "state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("error: unable to register topic\n");
        return NULL;
    }

    while (1) {
        char *status = "work";

        if (emcute_pub(&topic, status, strlen(status), EMCUTE_QOS_1) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }

        printf("Published: %s\n", status);
        xtimer_sleep(PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    /* Start the emcute thread */
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute_thread");
    
    return 0;
}