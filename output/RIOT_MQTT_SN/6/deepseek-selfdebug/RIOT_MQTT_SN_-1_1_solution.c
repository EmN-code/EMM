
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/af.h"
#include "net/conn/udp.h"
#include "net/netif.h"
#include "shell.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ADDR "47.102.103.1"
#define TOPIC "state"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t t;

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    local.port = EMCUTE_PORT;
    sock_udp_t sock;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    emcute_sub_t sub;
    memset(&sub, 0, sizeof(sub));
    sub.cb = NULL;
    sub.topic = &t;

    if (emcute_con(&sub, EMCUTE_ADDR, EMCUTE_PORT, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return NULL;
    }

    while (1) {
        const char *message = "work";  // or "not work" based on your logic
        emcute_pub(&sub, &t, message, strlen(message), 0);
        xtimer_usleep(INTERVAL);
    }

    return NULL;
}

int main(void)
{
    puts("Starting MQTT-SN client");

    t.name = TOPIC;
    t.id = 0;  // Topic ID will be assigned by the broker

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    return 0;
}
