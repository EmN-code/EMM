#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include "mutex.h"
#include "kernel_defines.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"

#define EMCUTE_ID           "gertrud"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT         (1885U)
#define EMCUTE_QOS          (0)  

#define TOPIC_NAME          "device/status"
#define STATE_MSG_WORK      "work"
#define STATE_MSG_NOT_WORK  "not work"

static char status_message[32];

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL; /* unreachable */
}

int main(void) {

    /* Copilot suggestions: zero initialize the topic.name, safely copy topic name below */
    memset(&topic, 0, sizeof(emcute_topic_t));
    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name) - 1);
v
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    /* Start emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* Connect to the MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%d\n",
               TOPIC_NAME, gw.port);
        return 1;
    }

    /* Register MQTT-SN topic */
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("error: unable to obtain topic ID\n");
        return 1;
    }

    int i = 0;
    while (1) {
        snprintf(status_message, sizeof(status_message), (i++ % 2) ? STATE_MSG_WORK : STATE_MSG_NOT_WORK);
        if (emcute_pub(&topic, status_message, strlen(status_message), EMCUTE_QOS) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }
        /* Wait for 5 seconds */
        xtimer_sleep(5);
    }
    return 0;
}