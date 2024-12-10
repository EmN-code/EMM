#include <ztimer.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <kernel.h>
#include <stdio.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define STATE_TOPIC "device/state"
#define STATE "work"
#define DELAY (5U * ZTIMER_MSEC_BASE)

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

static char stack[THREAD_STACKSIZE_MAIN];

static void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        emcute_pub(&topic, STATE, strlen(STATE), EMCUTE_QOS_0);
        ztimer_sleep(ZTIMER_MSEC_BASE, DELAY);
    }
    return NULL;
}

int main(void) {
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_handler, NULL, "pub_thread");

    emcute_reg(&topic);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC_BASE, DELAY);
    }

    return 0;
}