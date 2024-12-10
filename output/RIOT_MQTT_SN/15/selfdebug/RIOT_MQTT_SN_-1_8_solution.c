#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "xtimer.h"

#define BROKER_PORT     1883
#define BROKER_ADDRESS  "47.102.103.1"
#define PUB_TOPIC_NAME  "state"
#define EMA_TOPIC_ID    0x1010
#define STATE_WORK      "work"
#define STATE_NOT_WORK  "not work"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static int publish_state(const char *state) {
    unsigned flags = EMCUTE_QOS_0;
    int res = emcute_pub(&topic, state, strlen(state), flags);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish data\n");
    }
    return res;
}

static void *pub_thread(void *arg) {
    (void)arg;
    int res;

    topic.name = PUB_TOPIC_NAME;
    topic.id = EMCUTE_TID_NORMAL;

    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return NULL;
    }

    while (1) {
        publish_state(STATE_WORK);
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
        publish_state(STATE_NOT_WORK);
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    printf("MQTT-SN publisher started\n");

    msg_init_queue(queue, ARRAY_SIZE(queue));

    sock_udp_ep_t gw = {.port = BROKER_PORT};
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_ADDRESS);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to broker\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, pub_thread, NULL, "pub_thread");

    msg_receive(NULL);
    return 0;
}