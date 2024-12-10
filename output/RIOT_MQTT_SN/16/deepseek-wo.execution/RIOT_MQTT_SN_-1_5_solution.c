
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <thread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stddef.h>
#include <kernel_defines.h>

#define TOPIC_NAME "state"
#define MSG_CONTENT "work"
#define MSG_LEN sizeof(MSG_CONTENT) - 1
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define SLEEP_TIME (5 * 1000 * 1000UL) // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw;

int main(void)
{
    // Initialize the MQTT-SN client
    emcute_init();

    // Set up the gateway endpoint
    gw.family = AF_INET;
    gw.port = HTONS(BROKER_PORT);
    inet_aton(BROKER_IP, &gw.addr.ipv4);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: Could not connect to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: Could not register topic");
        return 1;
    }

    // Main loop to publish state every 5 seconds
    while (1) {
        if (emcute_pub(&topic, MSG_CONTENT, MSG_LEN, 0) != EMCUTE_OK) {
            puts("Error: Could not publish message");
        } else {
            puts("Message published successfully");
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_USEC, SLEEP_TIME);
    }

    return 0;
}
