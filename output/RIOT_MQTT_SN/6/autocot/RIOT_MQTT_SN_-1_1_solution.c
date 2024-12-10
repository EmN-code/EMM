#include <stdio.h>
#include <string.h>
#include "emcute.h"
#include "xtimer.h"

#define EMCUTE_ID "sensor"
#define EMCUTE_PORT 1883
#define EMCUTE_SERVER "47.102.103.1"
#define PUB_INTERVAL (5U * 1000000U)

static emcute_topic_t topic;
static char *state_msgs[] = {"not work", "work"};

static void *mqtt_client_thread(void *arg) {
    (void)arg;

    emcute_conf_t conf = {
        .id = EMCUTE_ID,
        .port = EMCUTE_PORT,
        .server = EMCUTE_SERVER
    };

    if (emcute_connect(&conf) != EMCUTE_OK) {
        puts("Error: unable to connect to server");
        return NULL;
    }

    topic.name = "state";
    topic.id = 0; // Will be set after subscription

    if (emcute_subscribe(&topic, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to subscribe to topic");
        return NULL;
    }

    unsigned counter = 0;
    while (1) {
        const char *state = state_msgs[counter % 2];
        if (emcute_publish(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data, state: %s\n", state);
        } else {
            printf("Published state: %s\n", state);
        }
        counter++;
        xtimer_sleep(PUB_INTERVAL / 1000000);
    }

    return NULL;
}

int main(void) {
    static char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_client_thread, NULL, "mqtt");
    return 0;
}