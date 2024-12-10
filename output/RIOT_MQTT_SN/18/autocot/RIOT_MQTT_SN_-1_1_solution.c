#include <stdio.h>
#include "xtimer.h"
#include "net/emcute.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_TOPIC "device/state"
#define PUB_MESSAGE "work or not work"
#define PUB_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;
static mqtt_client_t client;

static int publish_state(void) {
    msg_t reply;
    reply.content.ptr = &client;

    emcute_topic_t topic;
    topic.name = PUB_TOPIC;
    topic.id = 0; // ID will be assigned by the broker

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("[Error] Failed to register topic.\n");
        return -1;
    }

    res = emcute_pub(&topic, PUB_MESSAGE, strlen(PUB_MESSAGE), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("[Error] Failed to publish message.\n");
        return -1;
    }

    printf("[Info] Published state: %s\n", PUB_MESSAGE);
    return 0;
}

int main(void) {
    int res = emcute_con(BROKER_IP, BROKER_PORT);
    if (res != EMCUTE_OK) {
        printf("[Error] Failed to connect to broker at %s:%d\n", BROKER_IP, BROKER_PORT);
        return -1;
    }
    printf("[Info] Connected to broker at %s:%d\n", BROKER_IP, BROKER_PORT);

    // publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUB_INTERVAL);
    }

    // cleanup, although unreachable in current infinite loop structure
    emcute_discon();
    return 0;
}