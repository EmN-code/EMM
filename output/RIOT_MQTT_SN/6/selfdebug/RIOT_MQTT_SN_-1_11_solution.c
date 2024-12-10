#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "msg.h"

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "device/status"
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)

static char stack[STACK_SIZE];
static msg_t queue[8];

int main(void) {
    /* configure the network address */
    sock_udp_ep_t gw;
    if (inet_pton(AF_INET, BROKER_ADDRESS, &gw.addr.ipv4) != 1) {
        puts("Error: unable to parse gateway address");
        return 1;
    }
    gw.port = BROKER_PORT;
    gw.family = AF_INET;

    /* setup our subscription to the topic */
    emcute_topic_t topic;
    topic.name = MQTT_TOPIC;

    /* initialize emcute library */
    msg_init_queue(queue, ARRAY_SIZE(queue));
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to the gateway");
        return 1;
    }

    while (1) {
        /* publish a message every 5 seconds */
        if (emcute_pub(&topic, "work", strlen("work"), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }
        xtimer_sleep(5);
    }

    return 0;
}