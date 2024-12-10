#include <stdio.h>
#include <string.h>
#include "net/asymcute_conf.h"
#include "emcute.h"
#include "thread.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MQTT_DATA "work or not work"
#define DELAY 5000

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void *publisher_thread(void *arg) {
    (void)arg;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        /* Publish state message to the broker */
        if (emcute_pub(&topic, MQTT_DATA, strlen(MQTT_DATA), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("[error] unable to publish message");
        }
        /* Wait for the next cycle */
        xtimer_periodic_wakeup(&last_wakeup, DELAY);
    }
    return NULL;
}

int main(void) {
    /* Initialize the MCUte library */
    if (emcute_init(stack, sizeof(stack), EMCUTE_ID) != EMCUTE_OK) {
        puts("[error] unable to initialize emcute");
        return 1;
    }

    /* Connect to the MQTT-SN broker */
    sock_udp_ep_t gw = {.family = AF_INET, .port = BROKER_PORT};
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP);
    if (emcute_con(&gw, true, NULL) != EMCUTE_OK) {
        puts("[error] unable to connect to broker");
        return 1;
    }

    /* Subscribe to the topic */
    topic.name = TOPIC_NAME;
    if (emcute_sub(&topic, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("[error] unable to subscribe to topic");
        return 1;
    }

    /* Launch the publisher thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, publisher_thread, NULL, "publisher");

    return 0;
}