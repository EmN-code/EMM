
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
static emcute_topic_t topic;

void publish_state(void) {
    static const char *states[] = { "work", "not work" };
    static int state_index = 0;

    const char *state = states[state_index];
    state_index = (state_index + 1) % 2;

    printf("Publishing state: %s\n", state);

    if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
    }
}

int main(void) {
    // Initialize the network stack
    if (net_init() < 0) {
        puts("Error: unable to initialize the network stack");
        return 1;
    }

    // Convert the broker IP address from string to binary form
    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid broker IP address");
        return 1;
    }

    // Start the EMCUTE thread
    thread_create(emcute_stack, sizeof(emcute_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_run, NULL, "emcute");

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to the MQTT-SN broker");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register the topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
