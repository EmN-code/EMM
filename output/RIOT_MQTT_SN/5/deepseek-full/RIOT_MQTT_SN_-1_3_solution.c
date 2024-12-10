
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <xtimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work or not work"
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
static emcute_topic_t topic;

int main(void)
{
    /* Initialize the gateway endpoint */
    if (inet_aton(BROKER_IP, &gw.addr.ipv4) == 0) {
        puts("Error: invalid broker IP address");
        return 1;
    }

    /* Connect to the MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN broker");
        return 1;
    }

    /* Register the topic */
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: could not register topic");
        return 1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        if (emcute_pub(&topic, STATE_MESSAGE, sizeof(STATE_MESSAGE) - 1, 0) != EMCUTE_OK) {
            puts("Error: could not publish message");
        } else {
            puts("State published successfully");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
