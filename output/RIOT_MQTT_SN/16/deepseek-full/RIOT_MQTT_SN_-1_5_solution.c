
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <mma8x5x_params.h>
#include <net/sock/udp.h>
#include <mma8x5x.h>
#include <dht_params.h>
#include <event/thread.h>
#include <net/gcoap.h>
#include <MQTTClient.h>
#include <net/emcute.h>
#include <thread.h>
#include <net/sock/util.h>
#include <mma8x5x_regs.h>
#include <stddef.h>
#include <sys/socket.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_CONTENT "work or not work"

static emcute_topic_t topic;
static sock_udp_ep_t gw_ep = { .port = BROKER_PORT };

void publish_state(void) {
    int res;
    const char *msg = MSG_CONTENT;
    size_t msg_len = strlen(msg);

    res = emcute_pub(&topic, msg, msg_len, 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing message: %d\n", res);
    } else {
        printf("Message published successfully\n");
    }
}

int main(void) {
    int res;

    // Initialize the EMCUTE stack
    res = emcute_init();
    if (res != EMCUTE_OK) {
        printf("Error initializing EMCUTE: %d\n", res);
        return 1;
    }

    // Set up the gateway endpoint
    if (inet_aton(BROKER_ADDR, &gw_ep.addr.ipv4) == 0) {
        printf("Error parsing broker address\n");
        return 1;
    }

    // Connect to the MQTT-SN broker
    res = emcute_con(&gw_ep, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
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
        xtimer_sleep(5);
    }

    return 0;
}
