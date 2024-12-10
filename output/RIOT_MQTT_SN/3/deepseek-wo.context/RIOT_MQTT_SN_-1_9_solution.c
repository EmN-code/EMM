
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work"

static sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
static emcute_topic_t topic;

void publish_state(void) {
    char msg[20];
    strcpy(msg, MESSAGE);
    emcute_pub(&topic, msg, strlen(msg), 0);
}

int main(void) {
    // Initialize the network stack
    if (netdev_default == NULL) {
        puts("No default network device found");
        return 1;
    }
    netdev_t *dev = (netdev_t *)netdev_default;
    netdev_register(dev, NETDEV_ANY_IID, 0);
    netdev_set_up(dev);
    netdev_set_ipaddr(dev, IPV4_ADDR(192, 168, 1, 1));

    // Convert IP address string to binary form
    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("inet_pton failed");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Failed to connect to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Failed to register topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    // Disconnect from the MQTT-SN broker
    emcute_discon();

    return 0;
}
