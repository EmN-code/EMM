#include <string.h>  // Necessary for string manipulations, if any
#include "net/ipv4/addr.h"  // Required for inet_pton
#include "net/emcute.h"  // Header for emcute, our MQTT-SN library

#define EMCUTE_ID           "my_board"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define STATE_PUBLISH_DELAY (5000U)  // Publish state every five seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

// Define a topic for publishing
static emcute_topic_t state_topic = {
    .name = "state/work"
};

static void *emcute_thread(void *args) {
    (void)args;
    emcute_sub_t *sub = (emcute_sub_t *)args;

    // Configure the address and the port of the MQTT-SN broker
    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    // Connect to the broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway\n");
        return NULL;
    }

    // Register a topic
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        puts("Error: unable to obtain topic\n");
        return NULL;
    }

    while (1) {
        int res;
        char *state_msg = "work";

        // Publish a message every 5 seconds
        res = emcute_pub(&state_topic, state_msg, strlen(state_msg), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            puts("Error: unable to publish data\n");
        }
        xtimer_sleep(STATE_PUBLISH_DELAY / 1000);  // Convert ms to seconds
    }
    return NULL;
}

int main(void) {
    puts("MQTT-SN client example\n");

    // Initialize the emcute network stack
    memset(stack, 0, sizeof(stack));
    memset(queue, 0, sizeof(queue));

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    return 0;
}