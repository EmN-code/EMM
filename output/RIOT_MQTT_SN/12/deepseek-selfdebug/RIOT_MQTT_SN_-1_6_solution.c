
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };

static void publish_state(void) {
    static const char *states[] = { "work", "not work" };
    static int state_index = 0;

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0; // Topic ID will be assigned by the gateway

    // Register the topic
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return;
    }

    // Publish the state
    if (emcute_pub(&topic, states[state_index], strlen(states[state_index]), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        puts("State published");
    }

    // Toggle state
    state_index = 1 - state_index;
}

int main(void) {
    // Initialize the network stack
    if (net_init() < 0) {
        puts("Error: unable to initialize the network stack");
        return 1;
    }

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) != 1) {
        puts("Error: invalid IP address");
        return 1;
    }

    // Start the EMCUTE thread
    thread_create(emcute_stack, sizeof(emcute_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    // Connect to the MQTT-SN gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN gateway");
        return 1;
    }

    puts("Connected to MQTT-SN gateway");

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(MSG_INTERVAL);
    }

    return 0;
}
