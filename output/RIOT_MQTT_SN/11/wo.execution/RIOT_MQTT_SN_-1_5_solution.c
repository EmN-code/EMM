#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <thread.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <mutex.h>
#include <MQTTClient.h>
#include <event/thread.h>
#include <netinet/in.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>

#define EMCUTE_ID "mqtt-sn-client"
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static kernel_pid_t emcute_thread_pid;

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(1883, EMCUTE_ID);
    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    emcute_thread_pid = thread_create(emcute_stack, sizeof(emcute_stack), EMCUTE_PRIO, 0,
                                      emcute_thread, NULL, "emcute");

    emcute_topic_t topic;
    topic.name = "device/state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
    }

    while (1) {
        const char *msg = "work";
        if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
            printf("Failed to publish message\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}