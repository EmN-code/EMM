#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_ID           "device1"
#define MQTT_BROKER_ADDR    "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define MQTT_TOPIC          "device/state"
#define PUBLISH_INTERVAL    (5 * 1000000U) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static emcute_topic_t mqtt_topic;

static void publish_state(void) {
    const char *state = "work"; // This can be adjusted based on the actual state
    emcute_pub(&mqtt_topic, state, strlen(state), EMCUTE_QOS_0);
}

static int connect_to_broker(void) {
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = MQTT_BROKER_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_BROKER_ADDR);

    return emcute_con(&gw, true, NULL, NULL, 0, EMCUTE_CS);
}

int main(void) {
    puts("MQTT-SN connection example");

    msg_init_queue(queue, ARRAY_SIZE(queue));

    emcute_topic_t device_topic = { .name = MQTT_TOPIC };
    mqtt_topic = device_topic;

    if (connect_to_broker() != EMCUTE_OK) {
        puts("Connection to broker failed");
        return 1;
    }

    if (emcute_reg(&mqtt_topic) != EMCUTE_OK) {
        puts("Topic registration failed");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}