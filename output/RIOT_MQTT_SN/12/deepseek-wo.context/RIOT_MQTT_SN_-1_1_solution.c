
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
static emcute_sub_t sub;

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t gw = { .port = EMCUTE_PORT };
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, EMCUTE_ADDR) == NULL) {
        puts("Error: unable to parse IP address");
        return NULL;
    }
    gw.family = AF_INET;

    emcute_t *sock = emcute_new(&gw);
    if (sock == NULL) {
        puts("Error: unable to create EMCUTE socket");
        return NULL;
    }

    if (emcute_connect(sock, "riot_client", 60) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    topic.name = EMCUTE_TOPIC;
    if (emcute_reg(sock, &topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    char payload[20];
    while (1) {
        // Simulate state change
        const char *state = (random_uint32() % 2) ? "work" : "not work";
        snprintf(payload, sizeof(payload), "%s", state);

        if (emcute_pub(sock, &topic, payload, strlen(payload), EMCUTE_QOS) != EMCUTE_OK) {
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
