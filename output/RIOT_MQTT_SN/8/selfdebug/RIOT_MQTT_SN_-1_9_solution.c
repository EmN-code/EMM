#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "net/mqttsn.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT        (1883U)
#define EMCUTE_ID          "emcute-client"
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PUB_TOPIC   "state"

static emcute_topic_t topic;

static const char *state_msgs[] = { "work", "not work" };

static int pub_state(const char *state) {
    unsigned flags = EMCUTE_QOS_0;
    return emcute_pub(&topic, state, strlen(state), flags);
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    int res;

    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    topic.name = EMCUTE_PUB_TOPIC;

    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Could not connect to gateway: %d\n", res);
        return res;
    }

    while (1) {
        pub_state(state_msgs[0]);
        xtimer_sleep(5);
        pub_state(state_msgs[1]);
        xtimer_sleep(5);
    }
    return 0;
}