
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000 * 1000U) /* 5 seconds in microseconds */

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t topic;

void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t local = { .family = AF_INET, .port = EMCUTE_PORT };
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMCUTE_PORT, .addr = { .ipv4 = { .u8 = { 47, 102, 103, 1 } } } };

    if (sock_udp_create(&local, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return NULL;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return NULL;
    }

    char state[] = "work";
    while (1) {
        if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            puts("State published");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    puts("Starting MQTT-SN example");

    /* Initialize the network interface */
    if (netif_set_default(netif_iter(NULL)) == NULL) {
        puts("No default network interface found");
        return 1;
    }

    /* Start the emcute thread */
    thread_create(emcute_stack, sizeof(emcute_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    return 0;
}
