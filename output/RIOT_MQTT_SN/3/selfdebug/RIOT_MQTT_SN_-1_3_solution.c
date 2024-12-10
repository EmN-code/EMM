#include <string.h>
#include <stdio.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <xtimer.h>

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_TOPIC "state"
#define MQTT_MESSAGE "work"
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

static emcute_topic_t topic;

static void pub_state(void) {
    unsigned flags = EMCUTE_QOS_0;  // Using QoS level 0 for simplicity
    if (emcute_pub(&topic, MQTT_MESSAGE, strlen(MQTT_MESSAGE), flags) != EMCUTE_OK) {
        puts("Error: unable to publish data.");
    }
}

int main(void) {
    sock_udp_ep_t gw = {.family = AF_INET6, .port = BROKER_PORT};

    /* parse broker address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_ADDRESS) == NULL) {
        puts("Error: unable to parse broker address");
        return 1;
    }

    /* connect to broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return 1;
    }

    /* setup topic to publish to */
    topic.name = PUBLISH_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    /* main loop - publish state every 5 seconds */
    while (1) {
        pub_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}