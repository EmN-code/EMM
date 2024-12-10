#include <ztimer.h>
#include <stdio.h>
#include <string.h>
#include "net/emcute.h"
#include "net/conn/socket"  // This needs to be checked for file path correctness.

#define SERVER_ADDR  "47.102.103.1"
#define SERVER_PORT  1883
#define PUB_TOPIC    "state"
#define PUB_INTERVAL (5U * US_PER_SEC)

typedef enum {
    WORK,
    NOT_WORK
} work_state_t;

static emcute_topic_t topic;

void *publisher_thread(void *arg) {
    (void) arg;
    char state_msg[11];
    int counter = 0;
    while (1) {
        if (counter % 2) {
            sprintf(state_msg, "not work");
        } else {
            sprintf(state_msg, "work");
        }
        counter++;

        int ret = emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0);
        if (ret != EMCUTE_OK) {
            puts("[Error] Unable to publish message.");
        }
        ztimer_sleep(ZTIMER_USEC, PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = {.family = AF_INET6, .port = SERVER_PORT};
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, SERVER_ADDR);

    int res = emcute_con(&gw, true, NULL, NULL, 0, EMCUTE_CS);
    if (res != EMCUTE_OK) {
        puts("[Error] Unable to connect to the gateway.");
        return 1;
    }

    topic.name = PUB_TOPIC;
    topic.id = 0;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        puts("[Error] Unable to register topic.");
        return 1;
    }

    kernel_pid_t pub_tid = thread_create(
        stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, publisher_thread, NULL, "publisher");
    if (pub_tid < 0) {
        puts("[Error] Unable to create publisher thread.");
        return 1;
    }
    return 0;
}