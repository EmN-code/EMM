
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/netif.h"
#include "shell.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ID         "riot_esp32"
#define EMCUTE_TOPIC      "state"
#define EMCUTE_MSG_WORK   "work"
#define EMCUTE_MSG_NOT_WORK "not work"
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_sub_t sub;

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, "47.102.103.1") == NULL) {
        puts("Error: unable to parse gateway address");
        return NULL;
    }

    emcute_opt_t opts = { .node_id = EMCUTE_ID };
    if (emcute_run(&gw, &opts) != EMCUTE_OK) {
        puts("Error: unable to run emcute");
        return NULL;
    }

    while (1) {
        emcute_topic_t t;
        t.name = EMCUTE_TOPIC;
        if (emcute_reg(&t) != EMCUTE_OK) {
            puts("Error: unable to register topic");
            continue;
        }

        const char *msg = (xtimer_now().ticks32 % 2) ? EMCUTE_MSG_WORK : EMCUTE_MSG_NOT_WORK;
        if (emcute_pub(&t, msg, strlen(msg)) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }

        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    return 0;
}
