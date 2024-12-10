#include <net/sock/udp.h>
#include <net/af.h>
#include <kernel.h>

#include "msg.h"
#include "emcute.h"

// Constants
#define BROKER_IP      "47.102.103.1"
#define BROKER_PORT    1883
#define TOPIC_NAME     "device/state"
#define MAIN_QUEUE_SIZE (8)

// Function prototypes
static void *pub_thread(void *arg);

// Variables
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
static char stack[THREAD_STACKSIZE_MAIN];

// Main function
int main(void) {
    // Initialize the message queue
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Start the publisher thread
    (void)thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                        THREAD_CREATE_STACKTEST, pub_thread, NULL, "pub_thread");

    // Running the emCute library for MQTT-SN messaging
    emcute_run(BROKER_PORT, "device01");

    return 0;
}

static void *pub_thread(void *arg) {
    (void)arg;
    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0;  // Will be assigned by the gateway

    while (1) {
        char message[15];
        snprintf(message, sizeof(message), "work");

        printf("Publishing: %s\n", message);

        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish message.");
        }

        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}