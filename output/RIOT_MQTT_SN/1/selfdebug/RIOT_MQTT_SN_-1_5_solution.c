#include <net/ipv6/addr.h>
#include <net/af.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>

#include "emcute.h"

#define EMCUTE_PORT   1883                /**< Default port to connect to MQTT-SN */
#define SERVER_IP     "47.102.103.1"     /**< MQTT-SN broker address */

static sock_udp_ep_t remote = { .port = EMCUTE_PORT };

int main(void) {
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("ERROR: Unable to parse IP address\n");
        return 1;
    }

    emcute_topic_t topic;
    topic.name = "device/state";

    char *state_working = "work";
    char *state_not_working = "not work";

    unsigned flags = EMCUTE_QOS_0;

    while (1) {
        emcute_con(&remote, true, NULL, NULL, 0, 0);
        emcute_pub(&topic, state_working, strlen(state_working), flags);
        xtimer_sleep(5);
        emcute_pub(&topic, state_not_working, strlen(state_not_working), flags);
        xtimer_sleep(5);
        emcute_discon();
    }

    return 0;
}