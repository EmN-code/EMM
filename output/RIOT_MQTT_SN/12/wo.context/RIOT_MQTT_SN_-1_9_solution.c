#include <xtimer.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <string.h>

#include "emcute.h"

#define EMCUTE_PORT        (1883U)
#define EMCUTE_ID          "MQTT_Client"
#define STATE_TOPIC_NAME   "state/topic"
#define STATE_MSG_WORK     "work"
#define STATE_MSG_NOT_WORK "not work"
#define PUB_INTERVAL       (5U)  // Interval in seconds

typedef struct {
    const char *name;
    uint16_t id;
} emcute_topic_t;

static sock_udp_ep_t gateway = {
    .family = AF_INET,
    .port = EMCUTE_PORT
};

static emcute_topic_t topic;

void *publish_state(void *arg) {
    (void)arg;
    char *msg = STATE_MSG_WORK;
    
    inet_pton(AF_INET, "47.102.103.1", &gateway.addr.ipv4);

    while (1) {
        printf("Publishing state: %s\n", msg);
        if (emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish\n");
        }
        xtimer_sleep(PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    // Connect to Gateway
    if (emcute_con(&gateway, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to the MQTT-SN gateway\n");
        return 1;
    }

    // Register topic
    topic.name = STATE_TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    // Start publishing state
    publish_state(NULL);

    return 0;
}