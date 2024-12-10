#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/af.h>
#include <arpa/inet.h>  // Correctly included for inet_pton
#include <net/emcute.h>

#define BROKER_IP  "47.102.103.1"
#define BROKER_PORT 1883
#define NODE_ID     "esp32_node"
#define PUB_TOPIC   "state"
#define PUB_INTERVAL 5  // Publish every 5 seconds

static emcute_topic_t topic;

void publish_state(void) {
    const char *state = "work";  // Example state
    int res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish data [%d]\n", res);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    // Connect to MQTT-SN broker
    if (emcute_con(&gw, true, NODE_ID, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return 1;
    }

    // Register the topic
    topic.name = PUB_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Publish periodically every PUB_INTERVAL seconds
    while (1) {
        publish_state();
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}