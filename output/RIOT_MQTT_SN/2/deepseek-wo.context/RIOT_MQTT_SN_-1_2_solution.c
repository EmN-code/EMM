
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "net/netif.h"

#define EMCUTE_PORT 1883
#define TOPIC "state"
#define MSG "work or not work"

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

static void *pub_thread(void *arg)
{
    emcute_topic_t t;
    t.name = TOPIC;

    while (1) {
        xtimer_sleep(5);
        emcute_pub(&t, MSG, strlen(MSG), 0);
    }

    return NULL;
}

int main(void)
{
    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_t sock;

    /* Initialize the network interface */
    netif_t *netif = netif_iter(NULL);
    if (!netif) {
        puts("No network interface found.");
        return 1;
    }

    /* Set the gateway address */
    ipv4_addr_t gw_addr = { .u32 = IPv4_ADDR(47, 102, 103, 1) };
    gw.addr.ipv4 = gw_addr;

    /* Create a UDP socket */
    if (sock_udp_create(&sock, &local, &gw, 0) < 0) {
        puts("Error creating UDP socket.");
        return 1;
    }

    /* Connect to the MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker.");
        return 1;
    }

    /* Register the topic */
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error registering topic.");
        return 1;
    }

    /* Start the publishing thread */
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, pub_thread, NULL, "publisher");

    /* Main loop */
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
