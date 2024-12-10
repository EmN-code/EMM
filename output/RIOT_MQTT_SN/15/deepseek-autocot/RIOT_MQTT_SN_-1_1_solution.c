
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/netif.h"
#include "shell.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ADDR       "47.102.103.1"
#define EMCUTE_TOPIC      "state"
#define EMCUTE_QOS        0
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

    emcute_t *sock = emcute_new(&gw);
    if (sock == NULL) {
        puts("Error: unable to create EMCUTE socket");
        return NULL;
    }

    if (emcute_con(sock, NULL) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    topic.name = EMCUTE_TOPIC;
    if (emcute_sub(sock, &topic, EMCUTE_QOS) != EMCUTE_OK) {
        puts("Error: unable to subscribe to topic");
        return NULL;
    }

    while (1) {
        const char *state = (random_uint32() % 2) ? "work" : "not work";
        if (emcute_pub(sock, &topic, state, strlen(state), EMCUTE_QOS) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            printf("Published: %s\n", state);
        }

        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    puts("Starting MQTT-SN client");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  emcute_thread, NULL, "emcute");

    return 0;
}
