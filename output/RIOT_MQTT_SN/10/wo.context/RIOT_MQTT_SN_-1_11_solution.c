#include "net/gnrc/mqtt.h"
#include "emcute.h"
#include "xtimer.h"
#include "kernel_defines.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define STATE_MSG "work"
#define TOPIC_NAME "device/state"
#define PUB_INTERVAL (5 * US_PER_SEC)

static emcute_topic_t topic;

static int pub_state(void) {
    unsigned flags = EMCUTE_QOS_0;  // Quality of Service level 0
    if (emcute_pub(&topic, STATE_MSG, sizeof(STATE_MSG), flags) != EMCUTE_OK) {
        puts("Error: unable to publish data");
        return 1;
    }
    return 0;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        puts("Error: unable to parse gateway address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        pub_state();
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}