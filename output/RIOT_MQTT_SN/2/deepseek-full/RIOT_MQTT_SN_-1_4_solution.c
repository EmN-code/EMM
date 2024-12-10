
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
#include <net/ipv6/addr.h>
#include <mma8x5x.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <stdatomic.h>
#include <stdio.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work"
#define MESSAGE_LEN 4

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

static void _on_pub(void *arg, int reason, emcute_topic_t *topic)
{
    (void)arg;
    (void)reason;
    (void)topic;
}

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = { .family = AF_INET };

    // Convert IP address to binary form
    if (inet_aton(BROKER_IP, &gw.addr.ipv4) == 0) {
        puts("Error: invalid broker IP address");
        return 1;
    }

    // Create UDP socket
    if (sock_udp_create(&sock, &local, &gw, 0) < 0) {
        puts("Error: failed to create UDP socket");
        return 1;
    }

    // Connect to MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: failed to connect to MQTT-SN broker");
        return 1;
    }

    // Register topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: failed to register topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        if (emcute_pub(&topic, MESSAGE, MESSAGE_LEN, 0) != EMCUTE_OK) {
            puts("Error: failed to publish message");
        }
        thread_sleep(5 * 1000 * 1000); // Sleep for 5 seconds
    }

    return 0;
}
