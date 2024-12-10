#include <xtimer.h>  // RIOT timer functions for sleeping and delaying.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <thread.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define PUBLISH_INTERVAL (5U * 1000000U) // Publish interval in microseconds

static emcute_topic_t topic;

void publish_state(void) {
    static const char *state_work = "work";
    static const char *state_not_work = "not work";
    static bool state = true; // Initial state set to "work"

    while (1) {
        emcute_msg_t msg;
        memset(&msg, 0, sizeof(msg));

        // Toggle state
        msg.data = (state ? (void *)state_work : (void *)state_not_work);
        msg.len = strlen(msg.data);
        msg.type = EMCUTE_MSG_TYPE_PUBLISH;
        msg.flags = EMCUTE_MSG_FLAG_RETAIN;

        if (emcute_pub(&topic, &msg) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        else {
            printf("Published %s\n", (char *)msg.data);
        }
        state = !state; // Toggle state

        xtimer_usleep(PUBLISH_INTERVAL);
    }
}

int main(void) {
    int rc;
    sock_udp_ep_t gw = {
        .family = AF_INET6,
        .netif = SOCK_ADDR_ANY_NETIF,
        .port = MQTT_BROKER_PORT
    };

    char *gw_addr_str = MQTT_BROKER_IP;
    ipv6_addr_t gw_addr;

    if (ipv6_addr_from_str(&gw_addr, gw_addr_str) == NULL) {
        printf("Error: unable to parse gateway address\n");
        return 1;
    }

    memcpy(&gw.addr.ipv6, &gw_addr, sizeof(gw_addr));

    // Initialize the MQTT-SN client
    while (1) {
        rc = emcute.connect(&gw, true, NULL, NULL, 0);
        if (rc != EMCUTE_OK) {
            printf("Error: unable to connect to [%s]:%d\n", gw_addr_str, MQTT_BROKER_PORT);
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected to broker [%s]:%d\n", gw_addr_str, MQTT_BROKER_PORT);
            break;
        }
    }

    topic.name = MQTT_TOPIC;
    topic.id = 0;  // Using 0 means let the broker assign an ID
    if (emcute_sub(&topic, 0) != EMCUTE_OK) {
        printf("Error: unable to subscribe to %s\n", topic.name);
        return 1;
    }

    // Start state publishing
    publish_state();

    // Should never be reached
    return 0;
}