
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <stdatomic.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work or not work"
#define SLEEP_TIME 5000000  // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw_ep = SOCK_IPV4_EP_ANY;

void publish_state(void) {
    while (1) {
        int res = emcute_pub(&topic, MESSAGE, sizeof(MESSAGE) - 1, 0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish message: %d\n", res);
        } else {
            printf("Message published successfully\n");
        }
        thread_sleep(SLEEP_TIME);
    }
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    gw_ep.port = EMQX_SERVER_PORT;

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &gw_ep.addr.ipv4) <= 0) {
        printf("Invalid IP address\n");
        return 1;
    }

    // Create UDP socket
    if (sock_udp_create(&sock, &local, &gw_ep, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return 1;
    }

    // Connect to MQTT-SN gateway
    int res = emcute_con(&gw_ep, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN gateway: %d\n", res);
        return 1;
    }

    // Register topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return 1;
    }

    // Start publishing thread
    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, publish_state, NULL, "publisher");

    return 0;
}
