#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/emcute.h>
#include "thread.h"

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define STATE_WORK "work"
#define STATE_NOT_WORK "not work"
#define SLEEP_INTERVAL (5U * US_PER_SEC)

static emcute_sub_t subscriptions[1];
static char stack[THREAD_STACKSIZE_MAIN];

static void *publish_thread(void *arg) {
    (void)arg;
    emcute_topic_t topic;
    int res;

    strcpy(topic.name, MQTT_TOPIC);

    while (1) {
        const char *state = STATE_WORK;  // You can change between STATE_WORK and STATE_NOT_WORK to simulate changes
        res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("[error] unable to publish data
");
            return NULL;
        }
        printf("Published: %s\n", state);
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    msg_t queue[8];
    msg_init_queue(queue, 8);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publish_thread, NULL, "publish_thread");

    emcute_conf_t conf = {
        .id = "device",
        .port = MQTT_SN_BROKER_PORT,
        .addr = { .ipv4 = { 47, 102, 103, 1 }, },
    };

    if (emcute_int(&conf) != 0) {
        printf("[error] unable to initialize MCUtE
");
        return 1;
    }

    return 0;
}