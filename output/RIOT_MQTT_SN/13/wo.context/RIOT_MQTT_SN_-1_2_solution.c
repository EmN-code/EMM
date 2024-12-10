#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/ipv6/addr.h>       // For inet_pton
#include <net/emcute.h>          // For emcute functions, make sure it's linked against emcute

#define EMCUTE_PORT     (1883)
#define DEFAULT_PORT    (1883)
#define DEFAULT_IP      "47.102.103.1"
#define NODE_ID         "esp32_node"
#define PUB_TOPIC_NAME  "state"
#define PUB_INTERVAL    (5U)     // publish every 5 seconds

static emcute_topic_t topic;

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6 };
    char *state_msg = "work";

    if (inet_pton(AF_INET, DEFAULT_IP, &gw.addr.ipv4) != 1) {
        printf("Error: Unable to parse default IP
");
        return 1;
    }

    gw.port = DEFAULT_PORT;

    // Connect to the broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: Unable to connect to the broker
");
        return 1;
    }
    printf("Connected to broker at %s
", DEFAULT_IP);

    // Register and subscribe to the topic
    topic.name = PUB_TOPIC_NAME;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: Unable to register topic
");
        return 1;
    }
    printf("Registered topic %s
", topic.name);

    while (1) {
        // Publish state to the broker
        if (emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: Unable to publish data
");
        } else {
            printf("Published state: %s
", state_msg);
        }

        // sleep for +PUB_INTERVAL+ seconds
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}
