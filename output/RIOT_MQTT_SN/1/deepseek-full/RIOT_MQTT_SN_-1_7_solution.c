
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <kernel_defines.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work"

static emcute_sub_t sub;
static sock_udp_ep_t gw;
static emcute_topic_t topic;

void publish_state(void) {
    while (1) {
        int res = emcute_pub(&topic, MESSAGE, sizeof(MESSAGE) - 1, 0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish message: %d\n", res);
        } else {
            printf("Message published successfully\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
}

int main(void) {
    // Initialize the UDP endpoint for the gateway
    gw.family = AF_INET;
    gw.port = EMQX_SERVER_PORT;
    inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);

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
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0, (thread_task_func_t)publish_state, NULL, "publisher");

    return 0;
}
