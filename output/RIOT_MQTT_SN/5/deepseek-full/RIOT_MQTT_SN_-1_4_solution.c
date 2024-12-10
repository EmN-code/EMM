
#include <stdbool.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <mma8x5x_regs.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <stdlib.h>
#include <mutex.h>
#include <net/emcute.h>
#include <kernel_defines.h>
#include <mma8x5x.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <stdatomic.h>
#include <stdio.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work or not work"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_t sock;
static sock_udp_ep_t broker_ep;

static void _publish_state(void)
{
    while (1) {
        int res = emcute_pub(&topic, STATE_MESSAGE, sizeof(STATE_MESSAGE) - 1, 0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish message: %d\n", res);
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }
}

int main(void)
{
    // Initialize the UDP socket
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    local.port = 12345; // Arbitrary local port
    sock_udp_create(&sock, &local, NULL, 0);

    // Set up the broker endpoint
    broker_ep.family = AF_INET;
    broker_ep.port = BROKER_PORT;
    inet_pton(AF_INET, BROKER_IP, &broker_ep.addr.ipv4);

    // Connect to the MQTT-SN broker
    int res = emcute_con(&broker_ep, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to broker: %d\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return 1;
    }

    // Start publishing the state every 5 seconds
    _publish_state();

    return 0;
}
