#include <xtimer.h>  // For timer functionality
#include <net/sock/udp.h>  // Contains the sock_udp_t functions
#include <net/ipv6/addr.h>  // For IPv6 address manipulation
#include "emcute.h"  // Essential for connecting to MQTT-SN broker

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)  // 5 seconds

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };

void on_pub(void) {
    const char *data = "work";
    unsigned flags = EMCUTE_QOS_0;
    emcute_pub(&topic, data, strlen(data), flags);
}

int main(void) {
    char *stack = malloc(THREAD_STACKSIZE_DEFAULT);

    // Set broker IP and port
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, BROKER_IP);

    xtimer_ticks32_t last_pub = xtimer_now();

    // Connect to the MQTT-SN broker
    emcute_con(&gw, true, NULL, NULL, 0, 0);

    topic.name = "state";

    while (1) {
        on_pub();
        xtimer_periodic_wakeup(&last_pub, PUB_INTERVAL);
    }

    // Cleanup
    emcute_discon();
    free(stack);

    return 0;
}