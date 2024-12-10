#include <ztimer.h>  // Timer for sleeping
#include <stdio.h>   // Standard I/O
#include <string.h>  // For handling strings
#include <stdlib.h>  // Standard library for general purposes
#include <emcute.h>  // Emcute for MQTT-SN

#define MAIN_QUEUE_SIZE     (8)
#define SERVER_ADDR         "47.102.103.1"
#define SERVER_PORT         1883
#define PUB_INTERVAL        (5U)  // Interval to publish state, in seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

static void *publish_state(void *arg) {
    (void)arg;
    emcute_topic_t topic;
    topic.name = "state";
    topic.id = 0;
    char *state_msg = "work";

    // Attempt to publish every PUB_INTERVAL seconds
    while (1) {
        int res = emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            puts("Publish failed");
        }
        // Sleep for the specified interval
        ztimer_sleep(ZTIMER_MSEC, PUB_INTERVAL * 1000U);
    }
    return NULL;
}

int main(void) {
    // Initialize message queue
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    
    // Emcute connection parameters
    sock_udp_ep_t gw = { .family = AF_INET6, .addr = { .ipv6 = { 0 } }, .port = SERVER_PORT };
    inet_pton(AF_INET6, SERVER_ADDR, &gw.addr.ipv6);

    // Connect to gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway.");
        return 1;
    }

    // Create a thread to regularly publish the work state
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  publish_state, NULL, "publish state thread");

    return 0;
}