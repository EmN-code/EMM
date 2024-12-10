
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

    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    local.port = EMCUTE_PORT;
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMCUTE_PORT };

    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, EMCUTE_ADDR) == NULL) {
        puts("Error: unable to parse IPv6 address");
        return NULL;
    }

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return NULL;
    }

    emcute_sub_t sub;
    topic.name = TOPIC_NAME;
    topic.id = 0;

    if (emcute_sub(&sub, &topic, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to subscribe to topic");
        return NULL;
    }

    char payload[20];
    while (1) {
        strcpy(payload, "work");
        emcute_pub(&topic, payload, strlen(payload), EMCUTE_QOS_0);
        xtimer_usleep(PUBLISH_INTERVAL);
        strcpy(payload, "not work");
        emcute_pub(&topic, payload, strlen(payload), EMCUTE_QOS_0);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    puts("MQTT-SN example");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  emcute_thread, NULL, "emcute");

    return 0;
}
