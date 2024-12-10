
#include <stdbool.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <stdio.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    emcute_sub_t sub;
    char state[] = "work";

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4) != 1) {
        puts("Error: invalid IP address");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN gateway");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: could not register topic");
        return 1;
    }

    // Subscribe to the topic
    sub.topic = topic;
    sub.cb = _on_pub;
    if (emcute_sub(&sub, 0) != EMCUTE_OK) {
        puts("Error: could not subscribe to topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        if (emcute_pub(&topic, state, sizeof(state) - 1, 0) != EMCUTE_OK) {
            puts("Error: could not publish message");
        } else {
            printf("Published: %s\n", state);
        }
        thread_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}
