#include <stdio.h>
#include <string.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include "xtimer.h"

#define EMCUTE_ID           "device"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define KEEPALIVE          (60U)

defined(CONFIG_EMCUTE)
#define CONNECT_RETRIES    (3)
#define TIMEOUT_DURATION   (5U * US_PER_SEC)

static emcute_sub_t subscriptions[1];
static char stack[THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_MAIN];

static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(1883, EMCUTE_ID);
    return NULL;
}

static int connect_to_broker(void)
{
    sock_udp_ep_t gw = { .family = AF_INET6, .port = 1882 };  // Port set to 1883 for connection

    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, "47.102.103.1") == NULL) {
        printf("error: unable to parse IPv6 address\n");
        return 1;
    }

    for (int i = 0; i < CONNECT_RETRIES; i++) {
        if (emcute_con(&gw, true, NULL, NULL, 0, 0) == EMCUTE_OK) {
            printf("Successfully connected to broker\n");
            return 0;
        }
        else {
            printf("Failed to connect to broker (attempt %d)\n", i + 1);
            xtimer_sleep(TIMEOUT_DURATION);
        }
    }

    return 1;
}

int main(void)
{
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    if (connect_to_broker() != 0) {
        printf("Could not establish connection to broker\n");
        return 1;
    }

    emcute_topic_t topic;
    topic.name = "state";

    char payload[] = ("work");
    unsigned flags = EMCUTE_QOS_0;

    while (true) {
        if (emcute_pub(&topic, payload, strlen(payload), flags) == EMCUTE_OK) {
            printf("Published %s\n", payload);
        }
        else {
            printf("Publish failed\n");
        }
        xtimer_sleep(5);
    }
    return 0;
}