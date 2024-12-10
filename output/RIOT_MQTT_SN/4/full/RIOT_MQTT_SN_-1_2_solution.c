#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#define EMCUTE_PORT     (1883U)
#define NUMOFSUBS       (16U)
#define TOPIC_MAXLEN    (64U)
#define PUB_INTERVAL    (5U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_topic_t t;
    t.name = "state";

    while(1) {
        char data[20];
        sprintf(data, "work");
        if (emcute_reg(&t) != EMCUTE_OK) {
            puts("Error: unable to register topic");
            continue;
        }

        if (emcute_pub(&t, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish data");
        } else {
            printf("Published %s on topic %s\n", data, t.name);
        }
        xtimer_sleep(PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    printf("[MQTT-SN Publisher] Starting\n");

    /* initialize our subscription buffers */
    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));
    memset(topics, 0, (NUMOFSUBS * TOPIC_MAXLEN));

    /* start the emcute thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    /* connect the client to MQTT-SN broker */
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    inet_pton(AF_INET6, "47.102.103.1", &gw.addr.ipv6);

    if (emcute_con(&gw, false, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }

    puts("Connected to MQTT-SN broker\n");
    thread_yield();
    return 0;
}
