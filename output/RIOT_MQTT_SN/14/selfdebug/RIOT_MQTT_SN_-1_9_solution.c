#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define BROKER_IP     "47.102.103.1"
#define BROKER_PORT   1883
#define PUB_INTERVAL  (5U * US_PER_SEC)
#define CLIENT_ID     "esp32_client"
#define TOPIC_NAME    "device/state"

static emcute_topic_t topic;
static sock_udp_ep_t gateway = { .family = AF_INET6, .port = BROKER_PORT };

static void publish_state(const char* state) {
    unsigned flags = EMCUTE_QOS_0;
    int res = emcute_pub(&topic, state, strlen(state), flags);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish data (%d)\n", res);
    }
}

int main(void) {
    /* convert IP string to IPv6 address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&gateway.addr, BROKER_IP) == NULL) {
        puts("Error: unable to parse broker address");
        return -1;
    }

    /* connect to the MQTT-SN broker */
    if (emcute_con(&gateway, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return -1;
    }

    /* setup the topic we want to publish to */
    topic.name = TOPIC_NAME;
    emcute_reg(&topic);

    /* periodically publish the state */
    char *state = "work"; // or "not work" based on the actual state
    while (1) {
        publish_state(state);
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}