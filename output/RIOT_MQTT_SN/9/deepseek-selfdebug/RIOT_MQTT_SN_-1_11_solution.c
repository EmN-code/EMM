
#include <stdio.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t local = { .family = AF_INET, .port = EMCUTE_PORT };
    sock_udp_t sock;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
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
    emcute_topic_t topic;

    if (emcute_reg(&topic, TOPIC_NAME) != EMCUTE_OK) {
        puts("Error registering topic");
        return;
    }

    if (emcute_pub(&topic, states[state_index], strlen(states[state_index]), 0) != EMCUTE_OK) {
        puts("Error publishing state");
        return;
    }

    state_index = (state_index + 1) % 2;
}

int main(void)
{
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
