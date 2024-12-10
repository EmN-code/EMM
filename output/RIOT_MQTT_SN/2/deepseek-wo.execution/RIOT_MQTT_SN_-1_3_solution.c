
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <mutex.h>
#include <net/gcoap.h>
#include <net/sock/util.h>

#define TOPIC_NAME "state"
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw;

static void _publish_state(void)
{
    const char *state = "work";  // or "not work" based on your logic
    int res = emcute_pub(&topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing state: %d\n", res);
    } else {
        printf("State published: %s\n", state);
    }
}

static void _register_topic(void)
{
    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
    } else {
        printf("Topic registered: %s\n", topic.name);
    }
}

static void _connect_to_broker(void)
{
    gw.family = AF_INET;
    gw.port = BROKER_PORT;
    gw.netif = SOCK_ADDR_ANY_NETIF;

    if (sock_udp_ep_from_str(&gw, BROKER_IP) < 0) {
        printf("Error parsing broker address\n");
        return;
    }

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to broker: %d\n", res);
    } else {
        printf("Connected to broker at %s:%d\n", BROKER_IP, BROKER_PORT);
    }
}

int main(void)
{
    // Initialize the topic
    topic.name = TOPIC_NAME;
    topic.id = 0;  // This will be populated by emcute_reg

    // Connect to the broker
    _connect_to_broker();

    // Register the topic
    _register_topic();

    // Publish state every 5 seconds
    while (1) {
        _publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
