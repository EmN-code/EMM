
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

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

static void publish_state(void)
{
    static int state = 0;
    char payload[16];
    sprintf(payload, "work or not work: %s", state ? "work" : "not work");
    state = !state;

    if (emcute_pub(&topic, payload, strlen(payload), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        puts("Published state");
    }
}

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t local = { .family = AF_INET, .port = EMCUTE_PORT };

    if (sock_udp_create(&local, NULL, &gw, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return NULL;
    }

    emcute_sub_t sub;
    if (emcute_sub(&sub, &topic) != EMCUTE_OK) {
        puts("Error: unable to subscribe to topic");
        return NULL;
    }

    while (1) {
        xtimer_usleep(PUBLISH_INTERVAL);
        publish_state();
    }

    return NULL;
}

int main(void)
{
    puts("Starting MQTT-SN client");

    /* Initialize the network interface */
    netif_t *netif = netif_iter(NULL);
    if (!netif) {
        puts("Error: no network interface found");
        return 1;
    }

    /* Set the gateway address */
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, EMCUTE_ADDR) == NULL) {
        puts("Error: unable to parse gateway address");
        return 1;
    }

    /* Initialize the EMCUTE thread */
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    return 0;
}
