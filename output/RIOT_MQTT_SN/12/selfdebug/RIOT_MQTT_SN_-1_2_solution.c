#include <stdio.h>
#include <string.h>
#include <net/ipv6/addr.h>
#include "emcute.h"
#include "msg.h"
#include "xtimer.h"

enum { NUMOFSUBS = 16, MAXLEN = 64 };
static char stack[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];
static msg_t queue[NUMOFSUBS];
static emcute_sub_t subscriptions[NUMOFSUBS];
static char topic_name[] = "state";

static emcute_topic_t topics[MAXLEN];
static size_t num_topics = 0;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("Received publication: %.*s\n", (int)len, (char*)data);
}

int main(void) {
    /* initialize our subscription structs */
    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));
    /* initialize our topics */
    memset(topics, 0, (MAXLEN * sizeof(emcute_topic_t)));

    /* First, we configure the emcute network interface */
    inet_ntop(AF_INET6, "2001:db8::1", &ipv6_addr, sizeof(ipv6_addr));

    printf("Connecting to gateway at 47.102.103.1\n");
    sock_udp_ep_t gw = { .port = 1883, .family = AF_INET6 };
    inet_pton(AF_INET6, "47.102.103.1", &gw.addr.ipv6);
    
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway");
        return 1;
    }

    /* Create a topic for publishing data */
    emcute_topic_t t;
    t.name = topic_name;
    emcute_reg(&t);

    const char* state = "work or not work";

    while (1) {
        if (emcute_pub(&t, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        } else {
            printf("published: %s\n", state);
        }
        xtimer_sleep(5);
    }

    return 0;
}