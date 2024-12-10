// Include necessary headers in descending order
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <msg.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>

// Define constants
#define EMCUTE_ID       "emcute_client"
#define TOPIC_NAME      "device/state"
#define BROKER_PORT     1883
#define BROKER_ADDR     "47.102.103.1"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

// Declare global variables
static emcute_topic_t topic;

// Declare any necessary functions
static int publish_data(const char *message) {
    return emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
}

static int connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6 };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, BROKER_ADDR);
    gw.port = BROKER_PORT;

    return emcute_con(&gw, true, NULL, NULL, 0, 0);
}

int main(void) {
    // Initialize EMCUTE
    emcute_topic_t t;
    t.name = TOPIC_NAME;
    t.id = 0;
    topic = t;

    // Connect to broker
    if (connect_to_broker() != EMCUTE_OK) {
        printf("Error: unable to connect to broker\n");
        return 1;
    }

    // Register topic
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    // Publish state every five seconds
    while (1) {
        publish_data("work");
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    // Disconnect on cleanup (if ever reached)
    emcute_discon();
    return 0;
}

// Makefile content to ensure emcute library is included
// USEMODULE += emcute
// USEMODULE += ipv6
// USEMODULE += sock_.*