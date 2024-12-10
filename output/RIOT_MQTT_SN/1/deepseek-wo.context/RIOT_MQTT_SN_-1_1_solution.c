
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/af.h"
#include "net/conn/udp.h"
#include "net/sock/udp.h"
#include "net/netif.h"
#include "shell.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ADDR       "47.102.103.1"
#define TOPIC_NAME        "state"
#define PUBLISH_INTERVAL  (5 * 1000000U)  /* 5 seconds in microseconds */

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t topic;
static char emcute_buf[128];

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t local = { .family = AF_INET, .port = 0 };
    sock_udp_t sock;

    if (sock_udp_create(&sock, &local, &gw, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    emcute_sub_t sub;
    sub.cb = NULL;
    sub.topic = &topic;

    while (1) {
        /* Publish state every 5 seconds */
        const char *state = (random_uint32() % 2) ? "work" : "not work";
        printf("Publishing state: %s\n", state);
        snprintf(emcute_buf, sizeof(emcute_buf), "%s", state);
        emcute_pub(&sub, emcute_buf, strlen(emcute_buf) + 1, EMCUTE_QOS_0);

        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    /* Initialize the network interface */
    netif_t *netif = netif_iter(NULL);
    if (!netif) {
        puts("No network interface found");
        return 1;
    }

    /* Set up the gateway endpoint */
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, EMCUTE_ADDR) == NULL) {
        puts("Error parsing gateway address");
        return 1;
    }

    /* Initialize the EMCUTE thread */
    thread_create(emcute_buf, sizeof(emcute_buf), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    /* Register the topic */
    topic.name = TOPIC_NAME;
    topic.id = 0;
    emcute_reg(&topic);

    /* Start the EMCUTE client */
    emcute_run(netif, EMCUTE_PORT);

    return 0;
}
