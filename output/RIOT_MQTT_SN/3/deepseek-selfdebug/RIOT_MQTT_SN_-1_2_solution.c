
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "thread.h"

#define EMCUTE_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"

static char stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(EMCUTE_PORT, CLIENT_ID);
    return NULL;
}

static void publish_state(void)
{
    static const char *states[] = { "work", "not work" };
    static int state_index = 0;

    emcute_topic_t topic;
    if (emcute_reg(&topic, TOPIC) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return;
    }

    const char *state = states[state_index];
    state_index = (state_index + 1) % 2;

    if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
    } else {
        printf("Published: %s\n", state);
    }
}

int main(void)
{
    // Initialize the UDP endpoint for the gateway
    ipv4_addr_t gw_addr = { .u32 = IPV4_ADDR(47, 102, 103, 1) };
    gw.addr.ipv4 = gw_addr;

    // Start the emcute thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
