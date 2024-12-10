
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "net/netdev.h"
#include "shell.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static char stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .port = EMCUTE_PORT, .family = AF_INET };
static emcute_topic_t topic;

void on_pub_msg(void *arg, const emcute_topic_t *topic, void *data, size_t len)
{
    (void)arg;
    (void)topic;
    (void)data;
    (void)len;
    printf("Received message on topic %s\n", topic->name);
}

int main(void)
{
    /* Initialize the network interface */
    netdev_t *dev = netdev_get(0);
    if (dev == NULL) {
        puts("Error: no network device found");
        return 1;
    }

    /* Initialize EMCUTE */
    emcute_init();

    /* Set the gateway endpoint */
    ipv4_addr_t gw_addr = IPV4_ADDR(47, 102, 103, 1);
    gw.addr.ipv4 = gw_addr;

    /* Connect to the MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    /* Register the topic */
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    /* Subscribe to the topic */
    emcute_sub_t sub = { .topic = topic, .cb = on_pub_msg, .arg = NULL };
    if (emcute_sub(&sub, 0) != EMCUTE_OK) {
        puts("Error: unable to subscribe to topic");
        return 1;
    }

    /* Publish state every 5 seconds */
    char state[] = "work";
    while (1) {
        if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            puts("Published state: work");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
