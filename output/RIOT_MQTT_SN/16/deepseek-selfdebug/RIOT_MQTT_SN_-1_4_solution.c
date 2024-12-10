
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ID         "riot_esp32"
#define TOPIC_NAME        "state"
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .port = EMCUTE_PORT, .family = AF_INET };
static emcute_topic_t topic;

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_t sock;

    if (sock_udp_create(&sock, &local, &gw, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

static void publish_state(void)
{
    static const char *states[] = { "work", "not work" };
    static uint8_t state_index = 0;

    const char *state = states[state_index];
    state_index = (state_index + 1) % 2;

    if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
        puts("Error publishing state");
    } else {
        printf("Published state: %s\n", state);
    }
}

int main(void)
{
    // Initialize the network stack
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, "47.102.103.1") == NULL) {
        puts("Error parsing broker IP address");
        return 1;
    }

    // Start the emcute thread
    thread_create(emcute_stack, sizeof(emcute_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
