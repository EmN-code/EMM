#include <ztimer.h>
#include <stdio.h>
#include <string.h>
#include "net/emcute.h"

static emcute_topic_t topic;

static char broker_addr[] = "47.102.103.1";
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U * ZTIMER_MSEC_PER_SEC)
#define TOPIC_NAME "device/state"
#define CLIENT_ID "riot_device"

#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)
static char stack[THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF];
static emcute_sub_t subscriptions[CONFIG_EMCUTE_MAX_SUBS];

static void *publisher_thread(void *arg) {
    (void)arg;
    while (1) {
        const char *data = "work";
        if (emcute_pub(&topic, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Publication error!\n");
        }
        printf("Published: %s\n", data);
        ztimer_sleep(ZTIMER_MSEC, PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, broker_addr);

    /* connect to MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Unable to connect to [%s]:%d\n", broker_addr, (int)gw.port);
        return 1;
    }
    printf("Connected to [%s]:%d\n", broker_addr, (int)gw.port);

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Unable to register topic %s\n", TOPIC_NAME);
        return 1;
    }
    printf("Registered topic %s with ID %i\n", topic.name, topic.id);

    /* start the publishing thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, publisher_thread, NULL, "publisher");

    emcute_run(BROKER_PORT, CLIENT_ID);
    return 0;
}