#include "xtimer.h"
#include "net/emcute.h"
#include "msg.h"
#include <stdio.h>
#include <string.h>

#define BROKER_IPV6_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_PUBLISH_TOPIC "device/state"
#define MQTT_PUBLISH_DATA "work or not work"
#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS (16U)
#define TOPIC_MAXLEN (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[NUMOF_MSGS];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];
static emcute_topic_t t;

static int mqttsn_publish(const char* topic, const char* data) {
    t.name = topic;
    t.id = 0;

    if (emcute_connect(BROKER_IPV6_ADDRESS, BROKER_PORT, NULL, false) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%i\n", BROKER_IPV6_ADDRESS, BROKER_PORT);
        return 1;
    }

    if (emcute_pub(&t, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("error: unable to publish data");
        return 1;
    }

    return 0;
}

int main(void) {
    msg_init_queue(queue, NUMOF_MSGS);
    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));
    memset(topics, 0, (NUMOFSUBS * TOPIC_MAXLEN));

    // Start networking and emcute thread
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    while (1) {
        if (mqttsn_publish(MQTT_PUBLISH_TOPIC, MQTT_PUBLISH_DATA) != 0) {
            puts("Publish failed");
        }
        xtimer_sleep(INTERVAL / 1000000U);
    }

    return 0;
}