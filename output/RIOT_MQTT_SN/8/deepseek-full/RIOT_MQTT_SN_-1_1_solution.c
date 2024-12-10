
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/af.h"
#include "net/conn/udp.h"
#include "net/netif.h"
#include "shell.h"
#include "thread.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ADDR "47.102.103.1"
#define TOPIC "state"
#define BUFSIZE 128

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    uint8_t addr_str[IPV6_ADDR_MAX_STR_LEN];

    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, EMCUTE_ADDR) == NULL) {
        puts("Error: unable to parse IPv6 address");
        return NULL;
    }

    if (emcute_run(EMCUTE_PORT, "RIOT-MQTT-SN") != EMCUTE_OK) {
        puts("Error: unable to run emcute");
        return NULL;
    }

    if (emcute_con(&gw) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    topic.name = TOPIC;
    if (emcute_sub(&topic, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to subscribe to topic");
        return NULL;
    }

    while (1) {
        char payload[BUFSIZE];
        snprintf(payload, BUFSIZE, "work or not work");
        emcute_pub(&topic, payload, strlen(payload), EMCUTE_QOS_0);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    return 0;
}
