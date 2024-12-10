#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "device/state"
#define PUBLISH_INTERVAL (5U) // 5 seconds

static emcute_topic_t topic;

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_ADDRESS);

    // Connect to the MQTT-SN gateway
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        return 1; // Handle connection error
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        return 1; // Handle topic registration error
    }

    // Publish messages every 5 seconds
    char *message = "work or not work";
    while (1) {
        res = emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            return 1; // Handle publish error
        }
        xtimer_sleep(PUBLISH_INTERVAL);
    }
    return 0;
}