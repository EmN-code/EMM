
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include "emcute.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ADDR "47.102.103.1"
#define TOPIC_NAME "state"
#define MSG_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static char stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t topic;

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_t sock;

    if (sock_udp_create(&sock, &local, &gw, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    emcute_sub_t sub;
    sub.topic.name = TOPIC_NAME;
    sub.cb = NULL;

    if (emcute_reg(&sub.topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return NULL;
    }

    while (1) {
        const char *msg = "work";
        if (emcute_pub(&sub.topic, msg, strlen(msg), 0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            puts("Message published");
        }
        xtimer_usleep(MSG_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    /* Initialize the network interface */
    if (net_init() < 0) {
        puts("Error initializing network");
        return 1;
    }

    /* Convert IP address string to binary format */
    if (inet_pton(AF_INET, EMCUTE_ADDR, &gw.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    /* Start the EMCUTE thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    return 0;
}
