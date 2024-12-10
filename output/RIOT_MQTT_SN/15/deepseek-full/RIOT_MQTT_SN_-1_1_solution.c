
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
#define EMCUTE_TOPIC      "device/state"
#define EMCUTE_QOS        0
#define PUBLISH_INTERVAL  (5 * 1000000U)  /* 5 seconds in microseconds */

static emcute_sub_t sub;
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    printf("Received message on topic '%s' [%i]: ", topic->name, (int)topic->id);
    for (size_t i = 0; i < len; i++) {
        printf("%c", ((char *)data)[i]);
    }
    puts("");
}

static void publish_state(void)
{
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    emcute_topic_t t;
    t.name = EMCUTE_TOPIC;
    t.id = 0;  /* Topic ID will be assigned by the broker */

    if (emcute_pub(&t, states[state_index], strlen(states[state_index]), EMCUTE_QOS) != EMCUTE_OK) {
        puts("Error: unable to publish state");
    } else {
        puts("State published successfully");
    }

    state_index = (state_index + 1) % 2;  /* Toggle state */
}

int main(void)
{
    /* Initialize the network interface */
    netif_t *netif = netif_iter(NULL);
    if (!netif) {
        puts("Error: no network interface found");
        return 1;
    }

    /* Set up the gateway endpoint */
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, EMCUTE_ADDR) == NULL) {
        puts("Error: unable to parse gateway address");
        return 1;
    }

    /* Initialize the EMCUTE thread */
    emcute_run(netif, EMCUTE_PORT);

    /* Connect to the MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    /* Subscribe to the topic */
    sub.cb = on_pub;
    sub.topic.name = EMCUTE_TOPIC;
    if (emcute_sub(&sub, EMCUTE_QOS) != EMCUTE_OK) {
        puts("Error: unable to subscribe to topic");
        return 1;
    }
    puts("Subscribed to topic");

    /* Publish state every 5 seconds */
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
