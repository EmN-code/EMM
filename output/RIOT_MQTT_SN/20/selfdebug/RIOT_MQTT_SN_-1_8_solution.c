#include "net/emcute.h"
#include "xtimer.h"
#include "thread.h"
#include <stdio.h>

#define EMCUTE_ID "emcute-client"
#define EMCUTE_PORT 1883
#define MQTT_BROKER "47.102.103.1"
#define PUBLISH_INTERVAL (5U * US_PER_SEC) // 5 seconds

static emcute_topic_t state_topic;

void publish_state(void) {
    const char *msg = "work"; // message to publish
    emcute_pub(&state_topic, msg, strlen(msg), EMCUTE_QOS_0);
    puts("Published state: work");
}

int main(void) {
    // Create an emcute instance
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET6, MQTT_BROKER, &gw.addr.ipv6);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }

    state_topic.name = "device/state";
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}