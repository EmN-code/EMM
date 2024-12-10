#include "net/emcute.h"
#include "ztimer.h"
#include <string.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U * 1000000U) // Publish every 5 seconds (in microseconds)

// Define the topic and message
static emcute_topic_t topic;
static char *state_message = "work";

static void *pub_thread(void *arg) {
    (void)arg;

    while (1) {
        // Publish the message to the broker
        if (emcute_pub(&topic, state_message, strlen(state_message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish data");
        }

        // Sleep for PUB_INTERVAL
        ztimer_sleep(ZTIMER_USEC, PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    // Initialize emcute
    emcute_run(BROKER_PORT, "esp32-client");

    // Connect to the MQTT-SN broker
    sock_udp_ep_t gw = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF };
    inet_pton(AF_INET6, BROKER_IP, &gw.addr);
    gw.port = BROKER_PORT;

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway\n");
        return 1;
    }

    // Register the topic
    topic.name = "device/state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic\n");
        return 1;
    }

    // Start the publisher thread
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, pub_thread, NULL, "pub_thread");

    return 0;
}