
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work"

static void on_publish(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Received publish on topic '%s': %.*s\n", topic->name, (int)len, (char *)data);
}

int main(void) {
    sock_udp_ep_t gw = { .port = EMCUTE_PORT, .family = AF_INET };
    emcute_topic_t topic;
    emcute_sub_t sub;

    // Initialize the network stack
    netdev_t *netdev = netdev_get(0);
    if (netdev == NULL) {
        puts("Error: no network device found");
        return 1;
    }

    // Initialize EMCUTE
    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    // Set the gateway address
    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) <= 0) {
        puts("Error: invalid gateway address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: could not register topic");
        return 1;
    }

    // Subscribe to the topic
    sub.topic = topic;
    sub.cb = on_publish;
    if (emcute_sub(&sub, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: could not subscribe to topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        if (emcute_pub(&topic, STATE_MESSAGE, strlen(STATE_MESSAGE), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: could not publish message");
        } else {
            puts("Published state message");
        }
        xtimer_sleep(5);
    }

    return 0;
}
