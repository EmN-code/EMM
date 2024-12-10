#include <unistd.h>  // for sleep
#include <stdio.h>    // for printf
#include <net/ipv4/addr.h>  // for inet_pton
#include <net/sock/udp.h>  // for sock_udp_ep_t
#include "net/emcute.h"  // for emcute functions

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define PUB_TOPIC_NAME "state/topic"
#define CONFIG_EMCUTE_ID "my_mqttsn_client"
#define BUFFER_SIZE 64

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };

int main(void) {
    // Convert server IP string to network address
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    // Initialize emcute	emcute_con(&gw, true, NULL, NULL, 0, 0);

    // Register the topic
    topic.name = PUB_TOPIC_NAME;
    emcute_reg(&topic);

    // Infinite loop to publish the state every 5 seconds
    while (1) {
        const char *state = "work";  // replace logic as needed to determine state
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish state\n");
        }

        // Wait for 5 seconds
        sleep(5);
    }

    // Disconnect
    emcute_discon();
    return 0;
}