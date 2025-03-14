
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "emcute.h"

#define TOPIC "state"
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * 1000 * 1000U) /* 5 seconds in microseconds */

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

static void publish_state(void)
{
    static const char *states[] = { "work", "not work" };
    static int state_index = 0;
    const char *state = states[state_index];

    if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
    } else {
        puts("State published");
    }

    state_index = (state_index + 1) % 2;
}

int main(void)
{
    /* Initialize the network stack */
    if (net_init() < 0) {
        puts("Error: unable to initialize the network stack");
        return 1;
    }

    /* Convert the broker IP address from string to binary format */
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, BROKER_IP) == NULL) {
        puts("Error: unable to parse broker IP address");
        return 1;
    }

    /* Connect to the MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to the MQTT-SN broker");
        return 1;
    }

    /* Register the topic */
    topic.name = TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register the topic");
        return 1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
