
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
#define EMCUTE_ID         "riot_esp32"
#define EMCUTE_TOPIC      "state"
#define EMCUTE_QOS        0
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static char mqtt_sn_broker_addr[IPV6_ADDR_MAX_STR_LEN];
static sock_udp_ep_t gw = SOCK_IPV6_EP_ANY;
static emcute_sub_t sub;

static void publish_state(void)
{
    static int state = 0;
    const char *states[] = {"not work", "work"};
    char payload[10];

    sprintf(payload, "%s", states[state]);
    state = !state;  // Toggle state

    emcute_topic_t t;
    t.name = EMCUTE_TOPIC;
    t.id = 0;  // Topic ID will be assigned by the broker

    if (emcute_pub(&t, payload, strlen(payload), EMCUTE_QOS) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        printf("Published: %s\n", payload);
    }
}

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    local.port = EMCUTE_PORT;

    if (sock_udp_create(&sub.sock, &local, &gw, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return NULL;
    }

    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    return NULL;
}

int main(void)
{
    // Initialize network interface
    netif_t *netif = netif_iter(NULL);
    if (!netif) {
        puts("Error: no network interface found");
        return 1;
    }

    // Set MQTT-SN broker address
    ipv6_addr_from_str((ipv6_addr_t *)mqtt_sn_broker_addr, "47.102.103.1");
    gw.family = AF_INET;
    gw.port = EMCUTE_PORT;
    gw.netif = SOCK_ADDR_ANY_NETIF;
    memcpy(&gw.addr.ipv6, mqtt_sn_broker_addr, sizeof(gw.addr.ipv6));

    // Start the EMCUTE thread
    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
