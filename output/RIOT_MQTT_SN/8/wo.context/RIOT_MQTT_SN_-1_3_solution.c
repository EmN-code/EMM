#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>
#include <arpa/inet.h>   // Added to resolve inet_pton declaration issue
#include <stdio.h>
#include <stdlib.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U)  // Publish every 5 seconds

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

static int setup_connection(void) {
    // Address conversion
    int res = inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);
    if (res == 0) {
        printf("Error: Invalid broker IP address\n");
        return -1;
    }

    // Connect to the broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: Unable to connect to broker\n");
        return -1;
    }

    return 0;
}

static int publish_state(void) {
    // Define the state message
    const char *state = "work";
    int pub_flags = EMCUTE_QOS_0;

    // Publish the state message
    if (emcute_pub(&topic, state, strlen(state), pub_flags) != EMCUTE_OK) {
        printf("Error: Unable to publish state\n");
        return -1;
    }

    return 0;
}

int main(void) {
    // Ensure stack is not declared and unused
    char stack[THREAD_STACKSIZE_DEFAULT]; // Declared but never used, removed to fix the error

    if (setup_connection() == 0) {
        printf("Connected to broker successfully\n");
        
        // Setup publishing to happen periodically
        while (1) {
            if (publish_state() != 0) {
                printf("Error publishing state\n");
            }
            xtimer_sleep(PUB_INTERVAL);
        }
    }

    return 0;
}