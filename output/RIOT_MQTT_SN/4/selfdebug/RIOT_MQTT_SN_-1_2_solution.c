#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "xtimer.h"

#define BROKER_PORT 1883
#define BROKER_IP "47.102.103.1"
#define INTERVAL (5U * US_PER_SEC)  // 5 second interval

static emcute_topic_t topic;
static char topic_name[] = "state";

void publish_state(void) {
    const char *state = "work or not work";
    int res;

    res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish data (%d)\n", res);
    } else {
        printf("Published: %s\n", state);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP);
    
    memset(&topic, 0, sizeof(topic));
    topic.name = topic_name;

    // connect to broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway\n");
        return 1;
    }

    // register topic
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    // Main loop: publish periodically
    while (1) {
        publish_state();
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}