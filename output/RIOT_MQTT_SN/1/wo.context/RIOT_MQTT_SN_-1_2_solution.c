#include <xtimer.h>
#include "net/emcute.h"
#include "net/sock/udp.h"
#include <stdlib.h>
#include <stdio.h>

#define EMCUTE_ID     "device123"
#define SERVER_ADDR   "47.102.103.1"
#define SERVER_PORT   1883
#define TOPIC_NAME    "device/status"
#define STATUS_WORK   "work"
#define STATUS_NOTWORK "not work"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_MAIN];

static int connect_to_server(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, SERVER_ADDR);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("[Error] unable to connect to gateway: %d\n", res);
        return res;
    }

    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("[Error] unable to register topic: %d\n", res);
        return res;
    }
    return EMCUTE_OK;
}

static void *emcute_thread(void *arg) {
    (void)arg;
    static char status[16] = STATUS_WORK;
    size_t len = strlen(status);

    int res = connect_to_server();
    if (res != EMCUTE_OK) {
        return NULL;
    }

    while (1) {
        res = emcute_pub(&topic, status, len, EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("[Error] unable to publish data: %d\n", res);
        }
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }
    return NULL;
}

int main(void) {
    emcute_run(THREAD_CREATE_STACKTEST, stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, "emcute", emcute_thread, NULL, "emcute");
    return 0;
}