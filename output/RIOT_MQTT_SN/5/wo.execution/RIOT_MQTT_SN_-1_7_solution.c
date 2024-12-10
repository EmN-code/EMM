#include <xtimer.h>
#include <thread.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define TOPIC_NAME "device/state"
#define PUB_INTERVAL (5U * US_PER_SEC)  // 5 seconds

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_topic_t my_topic;

static void *mqttsn_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = {.family = AF_INET, .port = 1883};
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to gateway\n");
        return NULL;
    }

    my_topic.name = TOPIC_NAME;
    if (emcute_reg(&my_topic) != EMCUTE_OK) {
        printf("error: unable to register topic\n");
        return NULL;
    }

    while (1) {
        const char *state = "work";  // Either "work" or "not work",
        if (emcute_pub(&my_topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish message\n");
        }
        xtimer_sleep(PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqttsn_thread, NULL, "mqtt-sn");
    return 0;
}
