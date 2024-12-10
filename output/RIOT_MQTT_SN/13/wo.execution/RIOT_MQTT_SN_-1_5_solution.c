#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "thread.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "arpa/inet.h"
#include "kernel_defines.h"

#define EMCUTE_PORT     (1885U)
#define NUMOFSUBS       (16U)
#define TOPIC_MAXLEN    (64U)
#define MAIN_QUEUE_SIZE (8)
#define _IPV(address)     inet_addr(address)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[MAIN_QUEUE_SIZE];

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define PUBLISH_INTERVAL (5U)

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topic_stack[THREAD_STACKSIZE_MAIN];

/* MQTT-SN connection details */
static sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };

void *pub_thread(void *arg) {
    (void)arg;
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0; // QoS level 0

    strcpy(t.name, MQTT_TOPIC);
    emcute_reg(&t);

    while (1) {
        const char *state_msg = "work";
        if (emcute_pub(&t, state_msg, strlen(state_msg), flags) != EMCUTE_OK) {
            puts("Error: unable to publish");
        }
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, MAIN_QUEUE_SIZE);

    /* Set gateway address */
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr) != 1) {
        puts("Error: unable to parse gateway IP");
        return 1;
    }

    /* Start publisher thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  pub_thread, NULL, "pub_thread");

    emcute_con(&gw, true, NULL, NULL, 0, 0);

    return 0;
}
