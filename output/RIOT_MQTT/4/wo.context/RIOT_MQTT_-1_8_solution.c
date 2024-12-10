#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;

int publish_state(const char *state) {
    unsigned flags = EMCUTE_QOS_0;
    return emcute_pub(&topic, state, strlen(state), flags);
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .net = AF_INET6, .port = BROKER_PORT };
    memcpy(&gw.addr.ipv6, ipv6_addr_from_str("