
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000 * 1000U) /* 5 seconds in microseconds */

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

static void publish_state(void)
{
    static const char *states[] = { "work", "not work" };
    static uint8_t state_index = 0;

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0; /* Topic ID will be assigned by the broker */

    /* Register the topic */
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return;
    }

    /* Publish the state */
    if (emcute_pub(&topic, states[state_index], strlen(states[state_index]), 0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
        return;
    }

    /* Toggle state */
    state_index = (state_index + 1) % 2;
}

int main(void)
{
    /* Initialize the network stack */
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, "47.102.103.1") == NULL) {
        puts("Error: unable to parse gateway address");
        return 1;
    }

    /* Create a UDP socket for the gateway */
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &gw, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    /* Start the emcute thread */
    thread_create(emcute_stack, sizeof(emcute_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    /* Connect to the MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
