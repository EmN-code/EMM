
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
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw = SOCK_IPV6_EP_ANY;

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    local.port = EMCUTE_PORT;

    if (sock_udp_create(&sock, &local, &gw, 0) < 0) {
        puts("Error creating UDP sock");
        return NULL;
    }

    emcute_sub_t sub;
    sub.cb = NULL;
    sub.topic = &topic;

    if (emcute_sub(&sub) != EMCUTE_OK) {
        puts("Error subscribing to topic");
        return NULL;
    }

    while (1) {
        char payload[20];
        strcpy(payload, "work");  // or "not work" based on your logic
        emcute_pub(&topic, payload, strlen(payload) + 1, EMCUTE_QOS_0);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    puts("MQTT-SN example");

    /* Initialize the network interface */
    netif_t *netif = netif_iter(NULL);
    if (!netif) {
        puts("No network interface found");
        return 1;
    }

    /* Set up the gateway address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, EMCUTE_ADDR) == NULL) {
        puts("Error parsing gateway address");
        return 1;
    }
    gw.family = AF_INET;
    gw.port = EMCUTE_PORT;

    /* Initialize the topic */
    topic.name = TOPIC_NAME;
    topic.id = 0;  // Topic ID will be assigned by the gateway

    /* Start the EMCUTE thread */
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    return 0;
}
