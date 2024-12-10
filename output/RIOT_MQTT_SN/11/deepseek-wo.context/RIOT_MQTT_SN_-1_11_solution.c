
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "net/netdev.h"
#include "net/netdev/eth.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ADDR "47.102.103.1"
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .port = EMCUTE_PORT };

static void _publish_state(void)
{
    const char *state = "work"; // or "not work" based on your logic
    emcute_pub(&topic, state, strlen(state), 0);
}

int main(void)
{
    // Initialize network device
    netdev_t *dev = netdev_get(0);
    if (dev == NULL) {
        puts("Error: no network device found");
        return 1;
    }

    // Initialize EMCUTE
    emcute_init();

    // Convert IP address string to binary form
    ipv4_addr_t addr;
    if (ipv4_addr_from_str(&addr, EMCUTE_ADDR) == NULL) {
        puts("Error: unable to parse IP address");
        return 1;
    }

    // Set gateway endpoint
    gw.family = AF_INET;
    gw.netif = SOCK_ADDR_ANY_NETIF;
    gw.addr.ipv4 = addr;

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        _publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
