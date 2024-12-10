
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ADDR "47.102.103.1"
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static char stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = SOCK_IPV6_EP_ANY;

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    local.port = EMCUTE_PORT;

    if (sock_udp_create(&local, NULL, &gw, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    emcute_topic_t topic;
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
            puts("Message published");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    /* Initialize the RIOT network stack */
    if (netif_default == NULL) {
        puts("No default network interface found");
        return 1;
    }

    /* Set the gateway endpoint */
    gw.family = AF_INET;
    gw.port = EMCUTE_PORT;
    if (inet_pton(AF_INET, EMCUTE_ADDR, &gw.addr.ipv4) <= 0) {
        puts("Error parsing gateway address");
        return 1;
    }

    /* Start the EMCUTE thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    return 0;
}
