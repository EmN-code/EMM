#include <stdio.h>
#include <string.h>
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "net/emcute.h"  
#include "xtimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883

#define CLIENT_ID "mqtt_client"
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic = { .name = TOPIC_NAME, .id = 0 };

static void publish_state(const char *state) {
    if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("[error] unable to publish message");
    } else {
        puts("[info] message published: state");
    }
}

int main(void) {
    // Setup connection parameters
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    if (inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4) != 1) {
        puts("[error] unable to parse broker address");
        return 1;
    }

    // Connect to the broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("[error] unable to connect to broker\n");
        return 1;
    }
    puts("[info] connected to broker\n");

    // Register the topic
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("[error] unable to register topic");
        return 1;
    }
    puts("[info] topic registered\n");

    // Publish state every 5 seconds
    while (1) {
        publish_state("work");
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}