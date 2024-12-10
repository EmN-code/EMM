
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/netif.h"
#include "shell.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ADDR       "47.102.103.1"
#define EMCUTE_TOPIC      "device/state"
#define EMCUTE_QOS        0

static emcute_sub_t sub;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, EMCUTE_ADDR) == NULL) {
        puts("Error: unable to parse MQTT-SN broker address");
        return NULL;
    }

    emcute_t *cb = emcute_init(stack, sizeof(stack));
    if (cb == NULL) {
        puts("Error: unable to initialize EMCUTE");
        return NULL;
    }

    if (emcute_con(cb, &gw, true, NULL) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    while (1) {
        char state[20];
        if (xtimer_now().ticks32 % 10 == 0) {
            strcpy(state, "work");
        } else {
            strcpy(state, "not work");
        }

        emcute_topic_t topic;
        topic.name = EMCUTE_TOPIC;
        topic.id = 0;

        if (emcute_pub(cb, &topic, state, strlen(state), EMCUTE_QOS) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }

        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    return 0;
}
