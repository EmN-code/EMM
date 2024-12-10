
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT 1883
#define EMCUTE_TOPIC "state"
#define EMCUTE_MSG "work"
#define EMCUTE_MSG_LEN (sizeof(EMCUTE_MSG) - 1)
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t t;

static void _on_pub(void *arg, const emcute_topic_t *topic, void *data, size_t len)
{
    (void)arg;
    (void)topic;
    (void)data;
    (void)len;
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMCUTE_PORT };
    emcute_sub_t sub;
    int res;

    /* Initialize the network stack */
    netdev_t *netdev = netdev_get(0);
    if (netdev == NULL) {
        puts("Error: no network device found");
        return 1;
    }

    /* Set up the remote endpoint */
    if (inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4) <= 0) {
        puts("Error: invalid address");
        return 1;
    }

    /* Connect to the MQTT-SN gateway */
    res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN gateway (%d)\n", res);
        return 1;
    }

    /* Register the topic */
    t.name = EMCUTE_TOPIC;
    res = emcute_reg(&t);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic (%d)\n", res);
        return 1;
    }

    /* Subscribe to the topic */
    sub.topic = t;
    sub.cb = _on_pub;
    res = emcute_sub(&sub, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to subscribe to topic (%d)\n", res);
        return 1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        res = emcute_pub(&t, EMCUTE_MSG, EMCUTE_MSG_LEN, 0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish message (%d)\n", res);
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
