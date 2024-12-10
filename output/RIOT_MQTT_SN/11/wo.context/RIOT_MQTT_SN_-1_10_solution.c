#include <stdio.h>
#include <string.h>
#include <arpa/inet.h> // For inet_pton
#include <emcute.h> // Assuming emcute provides MQTT-SN functionality
#include <msg.h>
#include <xtimer.h> // For xtimer_sleep

#define EMCUTE_PORT     1883
#define EMCUTE_ID       "esp32"
#define NUMOFSUB        (1U) // Define the number of subscriptions

static emcute_topic_t subscriptions[NUMOFSUB]; // Declare subscriptions array

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8]; // Declare a message queue

// Forward declarations
static void on_pub(const emcute_topic_t *topic, void *data, size_t len);

// Callback function for subscription
static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    printf("Received publication\n");
}

int main(void) {
    // Initialize message queue
    msg_init_queue(queue, 8);

    // Initialize Network parameters
    sock_udp_ep_t gw = { .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    // Configure emcute
    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    // Main loop, publish "work" or "not work" every 5 seconds
    emcute_topic_t t;
    t.name = "state";
    while (1) {
        char *state = "work"; // Sample state
        emcute_pub(&t, state, strlen(state), EMCUTE_QOS_0);
        printf("Published: %s\n", state);
        xtimer_sleep(5);
    }

    return 0;
}
