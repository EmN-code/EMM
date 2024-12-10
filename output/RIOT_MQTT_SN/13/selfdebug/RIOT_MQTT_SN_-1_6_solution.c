#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include <emcute.h>
#include <stdio.h>
#include <string.h>
#include <xtimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define BUFSIZE 128

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static int publish_state(const char *state) {
    if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("error: unable to publish data");
        return 1;
    }
    puts("message sent");
    return 0;
}

int main(void) {
    sock_udp_ep_t gw = {.family = AF_INET6, .port = BROKER_PORT};
    inet_pton(AF_INET6, BROKER_IP, &gw.addr.ipv6);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect");
        return 1;
    }

    topic.name = "device/state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to register topic");
        return 1;
    }

    while (1) {
        publish_state("work");
        xtimer_sleep(5);
        publish_state("not work");
        xtimer_sleep(5);
    }

    emcute_discon();
    return 0;
}