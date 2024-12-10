#include <ztimer.h>  
#include <net/emcute.h>
#include <net/gnrc/netif.h>
#include <stdio.h>
#include <string.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define DEVICE_ID "esp32"

// Setup the topic for publishing
emcute_topic_t state_topic;
const char *state_topic_name = "device/state";

static void *publish_thread(void *arg) {
    (void)arg;

    char *state_msg = "work";

    while (1) {
        emcute_pub(&state_topic, state_msg, strlen(state_msg), EMCUTE_QOS_0);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    // define the remote endpoint
    sock_udp_ep_t gw = {
        .family = AF_INET6,
        .port = BROKER_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, BROKER_IP);

    // Initialize the emcute connection
    if (emcute_con(&gw, true, NULL, NULL, 0, EMCUTE_CS) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }

    // Register the topic
    state_topic.name = state_topic_name;
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Setup the thread to handle publishing
    thread_create(stack_pub, sizeof(stack_pub), THREAD_PRIORITY_MAIN - 1, 0,
                  publish_thread, NULL, "publish");

    // Execute emcute run
    emcute_run(BROKER_PORT, DEVICE_ID);

    return 0;
}

static char stack_pub[THREAD_STACKSIZE_MAIN];
