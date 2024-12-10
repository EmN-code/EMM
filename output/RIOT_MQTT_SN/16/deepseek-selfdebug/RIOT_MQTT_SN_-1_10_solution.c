
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
static emcute_topic_t topic;

void publish_state(void) {
    static const char *states[] = { "work", "not work" };
    static unsigned state_index = 0;
    const char *state = states[state_index];
    state_index = (state_index + 1) % 2;

    printf("Publishing state: %s\n", state);
    emcute_pub(&topic, state, strlen(state), 0);
}

int main(void) {
    /* Initialize the network stack */
    if (net_init() < 0) {
        puts("Error initializing network stack");
        return 1;
    }

    /* Convert IP address string to binary form */
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    /* Initialize EMCUTE */
    emcute_t *emcute_thread = thread_create(emcute_stack, sizeof(emcute_stack),
                                            THREAD_PRIORITY_MAIN - 1, 0,
                                            emcute_run, NULL, "emcute");
    if (emcute_thread == NULL) {
        puts("Error creating EMCUTE thread");
        return 1;
    }

    /* Connect to the MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }

    /* Register the topic */
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        publish_state();
        xtimer_usleep(MSG_INTERVAL);
    }

    return 0;
}
