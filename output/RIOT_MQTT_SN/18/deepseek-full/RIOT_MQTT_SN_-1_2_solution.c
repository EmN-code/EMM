
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/af.h"
#include "net/conn/udp.h"
#include "net/netif.h"
#include "shell.h"
#include "msg.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC "state"
#define STATE_MSG "work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t msg_queue[8];

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT, .addr = { .ipv4 = { 47, 102, 103, 1 } } };
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    emcute_con(&gw, true, NULL, NULL, 0, 0);
    return NULL;
}

static void publish_state(void)
{
    emcute_topic_t t;
    t.name = TOPIC;
    emcute_reg(&t);
    emcute_pub(&t, STATE_MSG, strlen(STATE_MSG), 0);
}

int main(void)
{
    msg_init_queue(msg_queue, 8);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
