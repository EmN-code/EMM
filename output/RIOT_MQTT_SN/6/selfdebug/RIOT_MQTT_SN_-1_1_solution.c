#include <thread.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <msg.h>
#include "net/emcute.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define MQTT_BROKER_IP      "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define MQTT_TOPIC          "state"
#define MQTT_MSG_WORK       "work"
#define MQTT_MSG_NOT_WORK   "not work"

static emcute_topic_t _mqtt_topic;

void *mqtt_thread(void *arg) {
    (void)arg;
    char *state_msgs[2] = { MQTT_MSG_WORK, MQTT_MSG_NOT_WORK };
    size_t state_index = 0;
    while (1) {
        puts("Publishing MQTT-SN message...");
        emcute_publish(&_mqtt_topic, state_msgs[state_index], strlen(state_msgs[state_index]), EMCUTE_QOS_0);
        state_index = (state_index + 1) % 2;
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    emcute_sub_t *subscriptions = NULL;

    /* initialize network interface */
    netif_init_t netif_init_config = {
        .ipv4 = { MQTT_BROKER_IP, MQTT_BROKER_PORT }
    };
    netif_init(&netif_init_config);

    /* setup application topic */
    _mqtt_topic.name = MQTT_TOPIC;
    _mqtt_topic.id = EMCUTE_ID_ANY;

    /* connect to MQTT-SN broker */
    if (emcute_con(MQTT_BROKER_IP, MQTT_BROKER_PORT, "riot-client", subscriptions, 0) != EMCUTE_OK) {
        puts("Error: Unable to connect to MQTT broker.");
        return 1;
    }

    /* create a thread to handle publishing */
    thread_create(stack_pub, sizeof(stack_pub), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}