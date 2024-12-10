#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include <stdio.h>
#include <stdlib.h>

// Constants
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define DEFAULT_PUB_TOPIC_NAME "state"
#define PUB_INTERVAL (5U * 1000000U) // in microseconds, 5 seconds

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

// Function to connect to the MQTT-SN broker
static int connect_to_broker(sock_udp_ep_t *remote) {
    while (1) {
        int ret = emcute_con(remote, true, NULL, NULL, 0, 0);
        if (ret != EMCUTE_OK) {
            printf("Connection to broker failed\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Connected to broker\n");
        break;
    }
    return EMCUTE_OK;
}

// Publish function
static void *publish(void *arg) {
    emcute_topic_t topic;
    char *pub_data = "work or not work";
    unsigned flags = EMCUTE_QOS_0;

    topic.name = DEFAULT_PUB_TOPIC_NAME;
    topic.id = 0; // will be populated

    while (1) {
        int ret = emcute_reg(&topic);
        if (ret != EMCUTE_OK) {
            printf("Register topic failed\n");
            xtimer_sleep(1);
            continue;
        }
        emcute_pub(&topic, pub_data, strlen(pub_data), flags);
        printf("Published: %s\n", pub_data);
        xtimer_usleep(PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    // Message queue initialization
    msg_init_queue(queue, sizeof(queue) / sizeof(msg_t));

    // Setup target address
    sock_udp_ep_t gw = {.family = AF_INET6, .port = MQTT_BROKER_PORT};
    inet_pton(AF_INET6, MQTT_BROKER_IP, &gw.addr.ipv6);

    // Connect to MQTT-SN broker
    if (connect_to_broker(&gw) != EMCUTE_OK) {
        printf("Could not connect to MQTT-SN broker\n");
        return 1;
    }

    // Start publishing thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publish, NULL, "publish");

    return 0;
}
