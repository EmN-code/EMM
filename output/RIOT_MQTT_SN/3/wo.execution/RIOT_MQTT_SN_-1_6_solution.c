#include <net/emcute.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <msg.h>
#include <assert.h>
#include <arpa/inet.h>

// Declare missing constants based on the error
#define EMCUTE_QOS_0 0
#define EMCUTE_OK 0

// Define a dummy type for emcute_topic_t if not provided in headers
typedef struct {
    char name[64];  // Assuming maximum topic name length
    int id;
} emcute_topic_t;

// Predefined addresses and topic
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "device/state"
#define STATE_MESSAGE "work or not work"

static sock_udp_ep_t remote;

int main(void) {
    // Initialize the message queue
    static msg_t queue[8];
    msg_init_queue(queue, 8);

    // Setup broker address
    if (inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4) != 1) {
        puts("error: unable to parse gateway address");
        return 1;
    }
    remote.port = BROKER_PORT;

    // Connect to broker
    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to broker");
        return 1;
    }

    // Prepare topic
    emcute_topic_t topic;
    memset(&topic, 0, sizeof(emcute_topic_t));
    strcpy(topic.name, TOPIC_NAME);

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to register topic");
        return 1;
    }

    // Publish message indefinitely
    while (1) {
        if (emcute_pub(&topic, STATE_MESSAGE, strlen(STATE_MESSAGE), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data");
        }
        sleep(5); // Wait for 5 seconds
    }
    return 0;
}