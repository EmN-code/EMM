
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

void publish_state(void) {
    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0; // Topic ID will be assigned by the broker

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return;
    }

    while (1) {
        res = emcute_pub(&topic, MESSAGE, sizeof(MESSAGE) - 1, 0);
        if (res != EMCUTE_OK) {
            printf("Error publishing message: %d\n", res);
        } else {
            printf("Message published successfully\n");
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
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
        printf("Error connecting to MQTT-SN gateway: %d\n", res);
        return 1;
    }

    // Create a thread to publish state every 5 seconds
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0, (thread_task_func_t)publish_state, NULL, "publisher");

    return 0;
}
