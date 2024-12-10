
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <xtimer.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work"
#define MESSAGE_LEN 4
#define FLAGS (0)  // No special flags for QoS and retain

static emcute_topic_t topic;

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET };
    emcute_topic_t topic;

    // Convert the broker address to a sock_udp_ep_t structure
    if (sock_udp_ep_from_str(&gw, BROKER_ADDR, BROKER_PORT) < 0) {
        puts("Error: unable to parse broker address");
        return 1;
    }

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

    // Publish the state every 5 seconds
    while (1) {
        if (emcute_pub(&topic, MESSAGE, MESSAGE_LEN, FLAGS) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            puts("Message published successfully");
        }

        // Sleep for 5 seconds
        xtimer_usleep(5 * 1000 * 1000);
    }

    return 0;
}
