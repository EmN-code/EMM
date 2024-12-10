#include "net/sock/udp.h"
#include "net/emcute.h"
#include "xtimer.h"
#include "stdio.h"

#define EMCUTE_PORT        (1883U)
#define TOPIC_NAME         "my_topic"
#define TOPIC_ID           (1U)
#define MSG_INTERVAL       (5U * US_PER_SEC)  // 5 seconds
#define REMOTE_ADDR        "47.102.103.1"

static emcute_topic_t topic;

static void *emcute_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    ipv6_addr_t addr;

    if (ipv6_addr_from_str(&addr, REMOTE_ADDR) == NULL) {
        puts("ERROR: unable to parse gateway address");
        return NULL;
    }

    memcpy(&gw.addr.ipv6, &addr, sizeof(addr));

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("ERROR: unable to connect to broker");
        return NULL;
    }

    emcute_topic_t t;
    t.name = TOPIC_NAME;
    t.id = TOPIC_ID;

    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("ERROR: unable to register topic");
        return NULL;
    }

    topic = t;

    while (1) {
        char *msg = "work";

        if (emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("ERROR: unable to publish message");
        }

        xtimer_sleep(MSG_INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_create(emcute_stack, sizeof(emcute_stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    return 0;
}