
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000 * 1000U) /* 5 seconds in microseconds */

static emcute_topic_t topic;

static void on_pub_msg(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    (void)data;
    (void)len;
    printf("Received message on topic %s\n", topic->name);
}

int main(void)
{
    sock_udp_ep_t gw = { .port = EMCUTE_PORT };
    emcute_sub_t sub;
    char *addr_str = "47.102.103.1";
    uint8_t addr[sizeof(struct in_addr)];

    /* Initialize the network interface */
    if (net_init() < 0) {
        puts("Error initializing network interface");
        return 1;
    }

    /* Convert IP address from string to binary */
    if (inet_pton(AF_INET, addr_str, addr) <= 0) {
        puts("Error converting IP address");
        return 1;
    }
    gw.addr.ipv4.u8 = addr;

    /* Initialize emcute */
    emcute_init();

    /* Connect to the MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    /* Register the topic */
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    /* Subscribe to the topic */
    sub.topic = topic;
    sub.cb = on_pub_msg;
    if (emcute_sub(&sub, 0) != EMCUTE_OK) {
        puts("Error subscribing to topic");
        return 1;
    }

    /* Publish state every 5 seconds */
    char state[] = "work";
    while (1) {
        if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            puts("Published state: work");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
