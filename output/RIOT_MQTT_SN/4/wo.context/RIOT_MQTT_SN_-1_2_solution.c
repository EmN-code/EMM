#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/ipv6/addr.h>
#include "net/emcute.h"
#include "xtimer.h"

#define EMCUTE_ID "my_device"
#define BROKER_PORT 1883
#define BROKER_IP "47.102.103.1"
#define SLEEP_INTERVAL (5U * US_PER_SEC)
#define TOPIC "status"

static emcute_topic_t topic;
static char message[64];

static int connect_to_broker(sock_udp_ep_t *gw) {
    while (emcute_con(gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Retrying to connect to the broker...");
        xtimer_sleep(2);
    }
    return EMCUTE_OK;
}

static void *publish_thread(void *arg) {
    (void)arg;
    while (1) {
        snprintf(message, sizeof(message), "work");
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Failed to publish message");
        }
        xtimer_usleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = {.family = AF_INET6, .port = BROKER_PORT};
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP);
    
    char stack[THREAD_STACKSIZE_MAIN];

    memset(&topic, 0, sizeof(topic));
    topic.name = TOPIC;
    emcute_reg(&topic);

    connect_to_broker(&gw);

    if (thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                      THREAD_CREATE_STACKTEST, publish_thread,
                      NULL, "publish") == KERNEL_PID_UNDEF) {
        puts("[error] failed to create thread");
        return 1;
    }

    emcute_run(BROKER_PORT, EMCUTE_ID);

    return 0;
}