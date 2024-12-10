
#include <net/emcute.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <xtimer.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work or not work"

static sock_udp_ep_t remote = { .family = AF_INET, .port = HTONS(EMQX_SERVER_PORT) };
static emcute_topic_t topic;

void publish_state(void) {
    static const char *msg = MESSAGE;
    int res;

    res = emcute_pub(&topic, msg, strlen(msg), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing message: %d\n", res);
    } else {
        printf("Message published successfully\n");
    }
}

int main(void) {
    int res;

    // Convert IP address string to binary form
    res = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (res != 1) {
        printf("Error converting IP address\n");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN gateway: %d\n", res);
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
        xtimer_sleep(5);
    }

    return 0;
}
