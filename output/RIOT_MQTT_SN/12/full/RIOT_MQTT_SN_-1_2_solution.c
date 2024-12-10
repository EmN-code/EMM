// Import necessary headers in the required order
#include "thread.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"

// Define constants
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define MQTT_BROKER_IP      "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define PUBLISH_INTERVAL    (5U)  // seconds
#define TOPIC_NAME          "state"
#define BUF_SIZE            (64U)

// Declare global variables
static char stack[THREAD_STACKSIZE_MAIN];
static emcute_topic_t topic;

// Thread function to publish MQTT-SN messages
void *publish_thread(void *arg) {
    (void)arg;
    char msg[BUF_SIZE];
    int count = 0;

    while (1) {
        // Toggle state message between "work" and "not work"
        if (count % 2 == 0) {
            snprintf(msg, sizeof(msg), "work");
        } else {
            snprintf(msg, sizeof(msg), "not work");
        }
        count++;

        // Publish the message
        int res = emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: Unable to publish data: %d\n", res);
        }

        // Sleep for the defined interval
        // for demonstration purposes, using intrinsic delay function
        for (volatile uint32_t i = 0; i < 10000000; i++);
    }

    return NULL;
}

// Main function to setup and connect to MQTT-SN broker
int main(void) {
    sock_udp_ep_t gw;
    memset(&gw, 0, sizeof(gw));

    // Setup the gateway address
    gw.port = MQTT_BROKER_PORT;
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        printf("Error: Malformed IP address\n");
        return -1;
    }

    // Initialize emcute
    int res = emcute.con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: Unable to connect to the broker: %d\n", res);
        return -1;
    }

    // Register the topic
    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: Unable to register topic: %d\n", res);
        return -1;
    }

    // Create the publish thread
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, publish_thread, NULL, "publish");

    // Keep main alive
    while (1) {}

    return 0;
}