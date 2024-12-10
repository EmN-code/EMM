#include <net/ipv6/addr.h>  // Include before net/sock/udp.h to resolve inet_pton
#include <net/sock/udp.h>
#include <emcute.h>
#include "xtimer.h"

#define EMCUTE_PORT     1883
#define SERVER_ADDR     "47.102.103.1"
#define TOPIC_NAME      "state"
#define PUB_MSG_WORK    "work"
#define PUB_MSG_NOWORK  "not work"
#define MSG_INTERVAL    5

static emcute_topic_t _topic;

void publish_state(const char *message) {
    int res = emcute_pub(&_topic, message, strlen(message), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish %s\n", message);
    } else {
        printf("Published: %s\n", message);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

    // Convert IP address
    inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4);

    // Connect to the gateway
    printf("Connecting to gateway...\n");
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway\n");
        return 1;
    } else {
        puts("Connected to gateway\n");
    }

    // Register topic
    _topic.name = TOPIC_NAME;
    if (emcute_reg(&_topic) != EMCUTE_OK) {
        puts("Error: unable to register topic\n");
        return 1;
    } else {
        printf("Registered topic: %s\n", _topic.name);
    }

    // Run infinite loop to publish messages
    while (1) {
        publish_state(PUB_MSG_WORK);
        xtimer_sleep(MSG_INTERVAL);
    }

    return 0;
}