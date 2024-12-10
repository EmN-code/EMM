
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define MSG "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_ep_t gw;
static emcute_topic_t topic;

static void _publish_state(void)
{
    char buf[16];
    strncpy(buf, MSG, sizeof(buf));
    emcute_pub(&topic, buf, strlen(buf), 0);
}

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

    // Initialize the gateway endpoint
    memset(&gw, 0, sizeof(gw));
    gw.family = AF_INET;
    gw.port = SERVER_PORT;

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid server IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        _publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
