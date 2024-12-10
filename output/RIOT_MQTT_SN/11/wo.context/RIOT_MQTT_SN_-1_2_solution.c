#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "msg.h"
#include "net/emcute.h"
#include "xtimer.h"

#define EMCUTE_PORT     (1883U)
#define MQTT_BROKER_IP  "47.102.103.1"
#define MQTT_TOPIC      "esp32/state"
#define PUB_MSG         "work or not work"
#define INTERVAL        (5U * US_PER_SEC)
#define MSG_QUEUE_SIZE  (8)

static msg_t _msg_queue[MSG_QUEUE_SIZE];

static emcute_topic_t _topic;

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;
    printf("Received publication from topic "%s": %s\n", topic->name, in);
}

int main(void)
{
    msg_init_queue(_msg_queue, MSG_QUEUE_SIZE);

    emcute_sub_t subscription = {
        .topic.name = MQTT_TOPIC,
        .cb = _on_pub
    };

    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = EMCUTE_PORT
    };

    // Resolve the broker address
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        puts("error: unable to parse broker address\n");
        return 1;
    }

    // Connect to the broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to broker\n");
        return 1;
    }

    // Register the topic
    if (emcute_reg(&_topic) != EMCUTE_OK) {
        puts("error: unable to register topic\n");
        return 1;
    }

    // Subscribe to the topic
    if (emcute_sub(&subscription, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("error: unable to subscribe to topic\n");
        return 1;
    }

    // Main publishing loop
    while (1) {
        if (emcute_pub(&_topic, PUB_MSG, strlen(PUB_MSG), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish message\n");
        }
        xtimer_sleep(INTERVAL);
    }

    return 0;
}