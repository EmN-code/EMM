#include "thread.h"
#include "stdio.h"
#include "net/emcute.h"
#include "event/thread.h"
#include "mma8x5x_regs.h"
#include "stdlib.h"
#include "net/gcoap.h"

#define EMCUTE_ID          1
#define EMCUTE_PORT        1883
#define PUBLISH_INTERVAL_USEC (5U * 1000000U) /* 5 seconds */
#define TOPIC_NAME         "device/state"

static kernel_pid_t emcute_thread_pid;
static char emcute_stack[THREAD_STACKSIZE_MAIN];

static void *emcute_thread(void *arg) {
    (void)arg;
   
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, "47.102.103.1");

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to MQTT-SN gateway\n");
        return NULL;
    }

    emcute_topic_t t;
    memset(&t, 0, sizeof(t));
    t.name = TOPIC_NAME;

    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("error: unable to register topic\n");
        return NULL;
    }

    const char *message = "work";

    while (1) {
        if (emcute_pub(&t, message, strlen(message), 0) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }
        xtimer_usleep(PUBLISH_INTERVAL_USEC);
    }
    return NULL;
}

int main(void) {
    emcute_thread_pid = thread_create(emcute_stack, sizeof(emcute_stack), THREAD_PRIORITY_MAIN - 1,
                                      THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    if (emcute_thread_pid == KERNEL_PID_UNDEF) {
        printf("error: unable to start thread\n");
        return 1;
    }

    return 0;
}