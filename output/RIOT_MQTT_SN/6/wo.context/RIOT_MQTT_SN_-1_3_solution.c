#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <net/ipv4/addr.h>
#include <arpa/inet.h>
#include <xtimer.h>
#include <net/emcute.h>

#define EMCUTE_ID        "RIOT"
#define EMCUTE_PORT      1883
#define EMCUTE_BROKER_IP "47.102.103.1"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t topic;
static char *state = "not work";

static int _emcute_con(sock_udp_ep_t *gw) {
    gw->family = AF_INET;
    gw->port = EMCUTE_PORT;
    if (inet_pton(AF_INET, EMCUTE_BROKER_IP, &gw->addr.ipv4) != 1) {
        puts("Error: unable to parse broker IP");
        return 1;
    }
    return emcute_con(gw, true, NULL, NULL, 0, 0);
}

static int _emcute_pub(const char *pub_state) {
    size_t len = strlen(pub_state);
    return emcute_pub(&topic, pub_state, len, EMCUTE_QOS_0);
}

int main(void) {
    sock_udp_ep_t gw = {.family = AF_INET, .port = EMCUTE_PORT};

    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_topic_t topic = {.name = "state", .id = 0};

    if (_emcute_con(&gw) != EMCUTE_OK) {
        puts("Error: unable to connect to the broker");
        return 1;
    }

    emcute_reg(&topic);
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        _emcute_pub(state);
        xtimer_periodic_wakeup(&last_wakeup, PUBLISH_INTERVAL);
    }

    return 0;
}
