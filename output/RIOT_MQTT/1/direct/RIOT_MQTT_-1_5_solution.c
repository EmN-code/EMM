#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_PORT         (1883U)
#define BROKER_IP "47.102.103.1"
#define MQTT_TOPIC         "state"
#define PUBLISH_INTERVAL   (5U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_sub_t subscriptions[1];

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(BROKER_IP, EMCUTE_PORT, stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1);
    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    printf("Publishing state every %d seconds\n", PUBLISH_INTERVAL);

    while (1) {
        char *message = "work or not work";
        emcute_topic_t topic;
        topic.name = MQTT_TOPIC;

        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        } else {
            printf("Published: %s\n", message);
        }

        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}