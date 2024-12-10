
#include <stdlib.h>
#include <stdio.h>
#include <net/emcute.h>
#include <thread.h>
#include <ztimer.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work or not work"

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_BROKER_PORT };

void publish_state(void) {
    while (1) {
        int res = emcute_pub(&topic, STATE_MESSAGE, sizeof(STATE_MESSAGE) - 1, 0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish state: %d\n", res);
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
}

int main(void) {
    // Initialize the network stack
    if (net_init() < 0) {
        printf("Error initializing network stack\n");
        return 1;
    }

    // Set the gateway address
    if (inet_pton(AF_INET, MQTT_SN_BROKER_IP, &gw.addr.ipv4) <= 0) {
        printf("Error parsing broker IP address\n");
        return 1;
    }

    // Connect to the MQTT-SN broker
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker: %d\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return 1;
    }

    // Create a thread to publish state every 5 seconds
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0, (thread_task_func_t)publish_state, NULL, "publish_state");

    return 0;
}
