// Include necessary headers in reverse alphabetical order
#include <ztimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <msg.h>

// Define constants
#define MQTT_PORT      (1883U)
#define MQTT_BROKER    "47.102.103.1"
#define MQTT_TOPIC     "device/state"
#define NODE_ID        "esp32"
#define EMCUTE_PRIO    (THREAD_PRIORITY_MAIN - 1)
#define QUEUE_SIZE     (8U)
#define MAX_MSG_SIZE   (128)
#define RETRY_INTERVAL (5U)

// Define global variables
static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[QUEUE_SIZE];

static emcute_topic_t topic;

// Function to connect to the MQTT-SN broker
static int connect_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = MQTT_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_BROKER);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res == EMCUTE_OK) {
        puts("Connected to broker");
    }
    else {
        printf("Connection failed with error code %d\n", res);
    }
    return res;
}

// Function to register a topic
static int register_topic(void) {
    strncpy(topic.name, MQTT_TOPIC, sizeof(topic.name));
    return emcute_reg(&topic);
}

// Function to publish a message
static void publish_state(const char *state) {
    if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error while publishing message");
    }
    else {
        printf("Published state: %s\n", state);
    }
}

// Main function
int main(void) {
    msg_init_queue(queue, QUEUE_SIZE);

    thread_create(emcute_stack, sizeof(emcute_stack), EMCUTE_PRIO, 0, emcute_run, NULL, "emcute");

    if (connect_broker() != EMCUTE_OK) {
        return 1;
    }

    if (register_topic() != EMCUTE_OK) {
        return 1;
    }

    while (1) {
        publish_state("work");
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}