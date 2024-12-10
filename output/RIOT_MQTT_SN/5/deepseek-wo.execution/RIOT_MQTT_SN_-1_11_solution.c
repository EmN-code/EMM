
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
#define MESSAGE "work"

static emcute_topic_t topic;

void publish_state(void) {
    const char *message = MESSAGE;
    int res;

    res = emcute_pub(&topic, message, strlen(message), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing message: %d\n", res);
    } else {
        printf("Message published successfully\n");
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    int res;

    // Convert IP address string to byte representation
    res = inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);
    if (res != 1) {
        printf("Error converting IP address\n");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
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

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        thread_sleep(5 * 1000 * 1000); // Sleep for 5 seconds
    }

    return 0;
}
