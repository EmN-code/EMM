#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <net/ipv4/addr.h>
#include <net/emcute.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)
#define TOPIC_NAME "state_topic"

static emcute_topic_t topic;

int main(void) {
    sock_udp_ep_t gateway = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    char *state_topic = TOPIC_NAME; // Use this variable in emcute_pub.
    char *state = "work";

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gateway.addr.ipv4) != 1) {
        puts("ERROR: unable to parse broker address");
        return 1;
    }

    if (emcute_con(&gateway, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("ERROR: unable to connect to broker");
        return 1;
    }

    topic.name = state_topic;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("ERROR: unable to register topic");
        return 1;
    }

    while (1) {
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("ERROR: unable to publish data");
        } else {
            puts("published successfully");
        }
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }
    
    emcute_discon();
    return 0;
}