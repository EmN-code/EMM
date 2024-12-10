#include <string.h> 
#include <stdio.h>
#include <net/sock/udp.h>
#include "emcute.h"
#include "xtimer.h"
#include "thread.h"

#define EMCUTE_ID           "device"
#define EMCUTE_PORT         (1883U)
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define CONNECT_RETRY_DELAY (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];
static char buf[128];
static emcute_sub_t subscriptions[3];
static emcute_topic_t topics[3];
static sock_udp_ep_t gw;

static void topic_cb(const emcute_topic_t *topic, 
                     void *data, size_t len) {
    char *in = (char *)data;
    (void)topic;
    printf("Received message: %.*s\n", (int)len, in);
}

static int publish(const char *topic, const char *data) {
    unsigned flags = EMCUTE_QOS_0;
    emcute_topic_t t;
    t.name = topic;

    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to register topic");
        return 1;
    }

    if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
        printf("error: unable to publish data to topic '%s'\n", topic);
        return 1;
    }

    printf("Published %s on topic %s\n", data, topic);
    return 0;
}

static void *emcute_thread(void *arg) {
    (void)arg;
    thread_sleep();

    while (1) {
        if (publish("status", "work") != 0) {
            puts("error: unable to publish topic");
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);

    // Initialize Gateway
    memset(&gw, 0, sizeof(gw));
    gw.family = AF_INET;
    gw.port = EMCUTE_PORT;
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, "47.102.103.1") == NULL) {
        puts("error: unable to parse gateway address");
        return 1;
    }

    // Initialize emcute
    emcute_run(stack, sizeof(stack), EMCUTE_PRIO, EMCUTE_ID);

    if (emcute_con(&gw, false, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway");
        return 1;
    }
    puts("Connected to gateway");

    // Create and start thread
    kernel_pid_t pid = thread_create(stack, sizeof(stack), 
                                     EMCUTE_PRIO, 0, 
                                     emcute_thread, NULL, "emcute_thread");
    if (pid <= KERNEL_PID_UNDEF) {
        puts("error: unable to start thread");
        return 1;
    }

    return 0;
}