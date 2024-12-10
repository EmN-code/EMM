#include <thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <arpa/inet.h>
#include <event/thread.h>

#define EMCUTE_ID           "esp32-client"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT         1883
#define EMCUTE_BUFSIZE      (1024U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

// Gateway address
static sock_udp_ep_t gw = {
    .family = AF_INET,
    .addr = "47.102.103.1",
    .port = EMCUTE_PORT
};

static emcute_topic_t topic;

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL; // this should never be reached
}

int main(void) {
    // Setup the thread for emcute
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    // Connect to the broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return 1;
    }

    // Register the topic
    strcpy(topic.name, "state");
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return 1;
    }

    // Publishing a message every 5 seconds
    while (1) {
        const char *data = "work";
        if (emcute_pub(&topic, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return 0; // Will never be reached
}
