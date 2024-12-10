
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <msg.h>
#include <thread.h>
#include <xtimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_QUEUE_SIZE 8
#define PUBLISH_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];
static emcute_topic_t topic;
static sock_udp_ep_t gw = { .port = BROKER_PORT, .family = AF_INET };

void publish_state(void) {
    const char *state = "work"; // or "not work" based on your logic
    emcute_pub(&topic, state, strlen(state), 0);
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Set up the gateway endpoint
    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid broker IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: could not register topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
