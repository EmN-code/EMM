
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ADDR "47.102.103.1"
#define TOPIC "state"
#define MSG "work"

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

static void publish_state(void)
{
    emcute_topic_t topic;
    if (emcute_reg(&topic, TOPIC) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return;
    }

    if (emcute_pub(&topic, MSG, strlen(MSG), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        puts("State published");
    }
}

int main(void)
{
    // Initialize the network interface
    netdev_t *netdev = netdev_get(0);
    if (netdev == NULL) {
        puts("Error: no network device found");
        return 1;
    }

    // Initialize emcute
    emcute_init();

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, EMCUTE_ADDR, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid IP address");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN gateway");
        return 1;
    }

    puts("Connected to MQTT-SN gateway");

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
