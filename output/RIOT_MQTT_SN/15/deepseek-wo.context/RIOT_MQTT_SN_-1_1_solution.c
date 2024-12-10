
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/netif.h"
#include "shell.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ID         "riot_esp32"
#define TOPIC_NAME        "state"
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;
static emcute_sub_t sub;

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    ipv4_addr_t gw_addr = { .u32 = { .addr = 0x6766022f } };  // 47.102.103.1 in hex
    gw.addr.ipv4 = gw_addr;

    if (emcute_con(&gw, EMCUTE_ID) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    while (1) {
        const char *state = (random_uint32() % 2) ? "work" : "not work";
        emcute_pub(&topic, state, strlen(state), 0);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    puts("RIOT MQTT-SN example");

    thread_create(emcute_stack, sizeof(emcute_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    return 0;
}
