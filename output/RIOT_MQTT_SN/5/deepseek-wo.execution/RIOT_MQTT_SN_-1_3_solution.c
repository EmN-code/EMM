
#include <stdio.h>
#include <thread.h>
#include <mutex.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <stdlib.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work or not work"

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

void publish_state(void) {
    while (1) {
        int res = emcute_pub(&topic, MESSAGE, sizeof(MESSAGE) - 1, 0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish message: %d\n", res);
        } else {
            printf("Message published successfully\n");
        }
        thread_sleep(5 * 1000 * 1000); // Sleep for 5 seconds
    }
}

int main(void) {
    // Initialize the UDP endpoint for the gateway
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    // Connect to the MQTT-SN gateway
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN gateway: %d\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return 1;
    }

    // Create a thread to publish the state every 5 seconds
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN, 0, (thread_task_func_t)publish_state, NULL, "publisher");

    return 0;
}
