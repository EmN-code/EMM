
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
#define EMCUTE_TOPIC      "device/state"
#define EMCUTE_QOS        0
#define PUBLISH_INTERVAL  (5 * 1000000U)  /* 5 seconds in microseconds */

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMCUTE_PORT };

    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, EMCUTE_ADDR) == NULL) {
        puts("Error: unable to parse IPv4 address");
        return NULL;
    }

    if (sock_udp_create(&sock_udp, &local, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return NULL;
    }

    emcute_sub_t sub;
    memset(&sub, 0, sizeof(sub));

    if (emcute_con(&sub, EMCUTE_ADDR, EMCUTE_PORT, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    if (emcute_reg(&sub, &topic, EMCUTE_TOPIC) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    while (1) {
        char payload[20];
        strcpy(payload, "work");  // Simulate state: "work" or "not work"

        if (emcute_pub(&sub, &topic, payload, strlen(payload), EMCUTE_QOS) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            printf("Published: %s\n", payload);
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
