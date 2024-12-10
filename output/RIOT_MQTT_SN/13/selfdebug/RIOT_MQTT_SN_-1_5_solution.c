#include <net/sock/udp.h>
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "msg.h"
#include "net/emcute.h"

#define SERVER_ADDRESS     "47.102.103.1"
#define SERVER_PORT        1883

#define TOPIC_NAME         "device/state"
#define PUBLISH_INTERVAL   (5U * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;

void *publish_thread(void *arg) {
    (void)arg;
    char *message = "work";
    int res;

    while (1) {
        res = emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish - %d\n", res);
        }
        // Wait for 5 seconds
        xtimer_usleep(PUBLISH_INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_q, ARRAY_SIZE(msg_q));
    memset(&topic, 0, sizeof(emcute_topic_t));

    sock_udp_ep_t gw = { .family = AF_INET6, .port = SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, SERVER_ADDRESS);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Unable to connect to gateway\n");
        return 1;
    }

    topic.name = TOPIC_NAME;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic\n");
        return 1;
    }

    kernel_pid_t publisher_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                                THREAD_CREATE_STACKTEST, publish_thread,
                                                NULL, "publish_thread");

    if (publisher_pid <= KERNEL_PID_UNDEF) {
        puts("Thread creation failed\n");
        return 1;
    }

    return 0;
}