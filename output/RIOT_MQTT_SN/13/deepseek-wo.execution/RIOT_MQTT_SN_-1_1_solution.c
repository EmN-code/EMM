
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/af.h"
#include "net/conn/udp.h"
#include "net/netif.h"
#include "shell.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ADDR       "47.102.103.1"
#define TOPIC_NAME        "state"
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    if (inet_pton(AF_INET, EMCUTE_ADDR, &gw.addr.ipv4) <= 0) {
        puts("Error: unable to parse gateway address");
        return NULL;
    }

    emcute_sub_t sub;
    memset(&sub, 0, sizeof(sub));

    if (emcute_con(&sub, &gw, EMCUTE_PORT, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    topic.name = TOPIC_NAME;
    topic.id = 0;  // Topic ID will be assigned by the broker

    while (1) {
        const char *state = (random_uint32() % 2) ? "work" : "not work";
        emcute_pub(&sub, &topic, state, strlen(state), 0);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    puts("MQTT-SN client starting");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    return 0;
}
