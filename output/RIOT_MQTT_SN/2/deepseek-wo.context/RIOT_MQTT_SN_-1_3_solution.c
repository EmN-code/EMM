
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"

static sock_udp_ep_t gw = { .port = EMCUTE_PORT };

void publish_state(void) {
    static const char *states[] = { "work", "not work" };
    static int state_index = 0;

    emcute_topic_t t;
    t.name = TOPIC;
    t.id = 0;

    char msg[128];
    snprintf(msg, sizeof(msg), "%s", states[state_index]);
    state_index = (state_index + 1) % 2;

    if (emcute_pub(&t, msg, strlen(msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        printf("Published: %s\n", msg);
    }
}

int main(void) {
    // Initialize the network stack
    if (ipv4_addr_from_str("47.102.103.1", (ipv4_addr_t *)&gw.addr.ipv4) == NULL) {
        puts("Error: unable to parse gateway address");
        return 1;
    }

    // Initialize the EMCUTE thread
    emcute_run(EMCUTE_PORT, CLIENT_ID);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    emcute_topic_t t;
    t.name = TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
