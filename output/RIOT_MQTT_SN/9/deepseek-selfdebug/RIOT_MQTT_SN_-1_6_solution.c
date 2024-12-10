
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_PORT       1883
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static char emcute_id[] = "riot_esp32_client";
static sock_udp_ep_t gw = { .port = EMCUTE_PORT };
static emcute_topic_t topic;

static void publish_state(void)
{
    const char *state = (xtimer_now().ticks32 % 2) ? "work" : "not work";
    printf("Publishing state: %s\n", state);

    if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
    }
}

int main(void)
{
    // Initialize the network stack
    netdev_t *netdev = netdev_get(0);
    if (netdev == NULL) {
        puts("Error: unable to get default network device");
        return 1;
    }

    // Set up the gateway endpoint
    IPV4_ADDR(&gw.addr.ipv4, 47, 102, 103, 1);
    gw.family = AF_INET;

    // Initialize emcute
    emcute_run(EMCUTE_PORT, emcute_id);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    // Register the topic
    topic.name = "state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }
    printf("Registered topic: %s with ID: %u\n", topic.name, topic.id);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
