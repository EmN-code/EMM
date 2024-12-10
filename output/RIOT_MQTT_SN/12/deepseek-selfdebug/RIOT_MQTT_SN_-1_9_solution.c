
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>  // Include this header for inet_pton
#include <net/emcute.h>
#include <net/sock/udp.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

static void publish_state(void) {
    static const char *states[] = { "work", "not work" };
    static int state_index = 0;
    const char *state = states[state_index];
    state_index = (state_index + 1) % 2;

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    emcute_reg(&topic);

    emcute_pub(&topic, state, strlen(state), 0);
}

int main(void) {
    // Initialize the network stack
    xtimer_sleep(1);

    // Convert the broker address to binary form
    if (inet_pton(AF_INET, BROKER_ADDR, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid broker address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN broker");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(MSG_INTERVAL);
    }

    // Disconnect from the broker
    emcute_discon();

    return 0;
}
