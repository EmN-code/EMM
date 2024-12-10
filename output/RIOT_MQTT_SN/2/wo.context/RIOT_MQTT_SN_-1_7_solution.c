#include "net/emcute.h"
#include "shell.h"
#include "thread.h"
#include "stdio.h"
#include "timex.h"
#include "ztimer.h"

// Define the MQTT-SN broker Endpoint
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

// Define the Emcute topic and message
#define TOPIC_NAME "device/state"
#define MESSAGE "work"

#define MAIN_QUEUE_SIZE (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

// Static variables for network configuration
static emcute_topic_t _topic;

static char stack[THREAD_STACKSIZE_MAIN];

// Function to connect to the broker
static int connect_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };
    inet_pton(AF_INET6, BROKER_IP, &gw.addr.ipv6);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%d\n", BROKER_IP, BROKER_PORT);
        return res;
    }
    return EMCUTE_OK;
}

// Function to publish a message to the topic
static int publish_state(void) {
    unsigned flags = EMCUTE_QOS_0;

    int res = emcute_pub(&_topic, MESSAGE, strlen(MESSAGE), flags);
    if (res != EMCUTE_OK) {
        printf("error: unable to publish data\n");
        return res;
    }
    return EMCUTE_OK;
}

// Main function to execute the program
int main(void) {
    // Initialize threading and emcute
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Set up the MQTT-SN connection
    if (connect_broker() != EMCUTE_OK) {
        return 1;
    }

    // Register the topic
    _topic.name = TOPIC_NAME;
    if (emcute_reg(&_topic) != EMCUTE_OK) {
        printf("error: unable to obtain topic ID\n");
        return 1;
    }

    // Loop to publish messages at intervals
    while (1) {
        if (publish_state() != EMCUTE_OK) {
            return 1;
        }

        // Wait for 5 seconds
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}