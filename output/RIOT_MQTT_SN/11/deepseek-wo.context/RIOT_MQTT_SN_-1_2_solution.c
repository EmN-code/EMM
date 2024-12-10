
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "thread.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ADDR       "47.102.103.1"
#define TOPIC_NAME        "state"
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = SOCK_IPV6_EP_ANY;

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    local.port = EMCUTE_PORT;

    if (sock_udp_create(&local, &gw, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    emcute_sub_t sub;
    sub.topic.name = TOPIC_NAME;
    sub.cb = NULL;

    if (emcute_reg(&sub.topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return NULL;
    }

    while (1) {
        const char *state = "work";
        if (emcute_pub(&sub.topic, state, strlen(state), 0) != EMCUTE_OK) {
            puts("Error publishing message");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    puts("EMCUTE example");

    /* set gateway address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, EMCUTE_ADDR) == NULL) {
        puts("Error parsing gateway address");
        return 1;
    }
    gw.family = AF_INET6;
    gw.port = EMCUTE_PORT;

    /* start the EMCUTE thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  emcute_thread, NULL, "emcute");

    return 0;
}
