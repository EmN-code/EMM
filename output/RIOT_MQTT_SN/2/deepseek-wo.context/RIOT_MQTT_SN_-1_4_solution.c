
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "state"
#define PUB_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;

static void publish_state(void)
{
    static int state = 0;
    const char *state_str = (state++ % 2 == 0) ? "work" : "not work";
    printf("Publishing state: %s\n", state_str);

    int res = emcute_pub(&topic, state_str, strlen(state_str), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing state: %d\n", res);
    }
}

int main(void)
{
    sock_udp_ep_t gw = { .port = BROKER_PORT };
    ipv4_addr_t broker_addr;

    // Convert the broker IP address from string to ipv4_addr_t
    if (ipv4_addr_from_str(&broker_addr, BROKER_IP) == NULL) {
        printf("Error parsing broker IP address\n");
        return 1;
    }

    gw.family = AF_INET;
    gw.netif = SOCK_ADDR_ANY_NETIF;
    gw.addr.ipv4 = broker_addr;

    // Connect to the MQTT-SN broker
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUB_INTERVAL);
    }

    return 0;
}
