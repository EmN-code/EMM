#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "xtimer.h"
#include "thread.h"

#define EMCUTE_PORT          (1883U)
#define EMCUTE_PRIO          (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS            (16U)
#define TOPIC_MAXLEN         (64U)

static emcute_sub_t subscriptions[NUMOFSUBS];
static char stack[THREAD_STACKSIZE_MAIN];
static char topic[TOPIC_MAXLEN] = "device/state";

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, "::0");
    return NULL;
}

static int publish(const char *topic, const char *data, int qos)
{
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    switch (qos) {
        case 1: flags = EMCUTE_QOS_1; break;
        case 2: flags = EMCUTE_QOS_2; break;
        default: flags = EMCUTE_QOS_0; break;
    }

    /* step 1: get topic id */
    t.name = topic;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID");
        return 1;
    }

    /* step 2: publish data */
    if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
        puts("error: unable to publish data");
        return 1;
    }

    puts("Published successfully!");

    return 0;
}

int main(void) {
    msg_init_queue(_main_msg_queue, ARRAY_SIZE(_main_msg_queue));

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* connect to broker */
    sock_udp_ep_t gw = {
        .family = AF_INET6,
        .port = EMCUTE_PORT
    };

    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, "47.102.103.1") == NULL) {
        puts("error: unable to parse IPv6 address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to broker");
        return 1;
    }

    while (1) {
        publish(topic, "work", 0);
        xtimer_sleep(5);
        publish(topic, "not work", 0);
        xtimer_sleep(5);
    }

    return 0;
}