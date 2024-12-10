#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <string.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_ID          "device123"
#define EMCUTE_PORT        (1883U)
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define PUB_INTERVAL       (5U) /* seconds */
#define BROKER_IP          "47.102.103.1"

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, queue, sizeof(queue) / sizeof(msg_t));
    return NULL;
}

static int publish(const char *topic_name, const char *message) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    /* setup topic */
    t.name = topic_name;

    /* publish data */
    if (emcute_pub(&t, message, strlen(message), flags) != EMCUTE_OK) {
        printf("error: unable to publish to topic '%s'\n", topic_name);
        return 1;
    }

    printf("Published %s on topic %s\n", message, topic_name);
    return 0;
}

static int connect(void) {
    sock_udp_ep_t gw = { .family = AF_INET6 };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, BROKER_IP);
    gw.port = EMCUTE_PORT;

    if (emcute_con(&gw, true, EMCUTE_ID, NULL, NULL, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%d\n", BROKER_IP, (int)gw.port);
        return 1;
    }

    printf("Connected to [%s]:%d\n", BROKER_IP, (int)gw.port);
    return 0;
}

int main(void) {
    memset(stack, 0, sizeof(stack));
    memset(queue, 0, sizeof(queue));

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    printf("Waiting for interface to be up...\n");
    xtimer_sleep(3);

    if (connect() != 0) {
        return 1;
    }

    while (1) {
        publish("state", "work");
        xtimer_sleep(PUB_INTERVAL);
        publish("state", "not work");
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}
