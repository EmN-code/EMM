
#include <stdlib.h>
#include <net/gcoap.h>
#include <dht.h>
#include <net/sock/udp.h>
#include <kernel_defines.h>
#include <stdbool.h>
#include <mma8x5x.h>
#include <net/ipv6/addr.h>
#include <stdio.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <dht_params.h>
#include <stdatomic.h>
#include <thread.h>
#include <mutex.h>
#include <mma8x5x_regs.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work or not work"

static emcute_topic_t topic;
static sock_udp_ep_t gw_ep = { .family = AF_INET, .port = EMQX_SERVER_PORT };

void publish_state(void) {
    while (1) {
        int res = emcute_pub(&topic, MESSAGE, sizeof(MESSAGE) - 1, 0);
        if (res != EMCUTE_OK) {
            printf("Error publishing message: %d\n", res);
        }
        thread_sleep(5 * 1000 * 1000); // Sleep for 5 seconds
    }
}

int main(void) {
    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &gw_ep.addr.ipv4) != 1) {
        puts("Error parsing IP address");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    int res = emcute_con(&gw_ep, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN gateway: %d\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    // Create a thread to publish state every 5 seconds
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, publish_state, NULL, "publisher");

    return 0;
}
