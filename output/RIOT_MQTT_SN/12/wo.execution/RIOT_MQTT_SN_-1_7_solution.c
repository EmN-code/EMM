#include "ztimer.h"
#include "msg.h"
#include "stdlib.h"
#include "net/emcute.h"
#include "xtimer.h"

#define EMCUTE_PORT     (1885U)
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define TOPIC_MAXLEN    (64U)
#define MQ_SN_BROKER_IP "47.102.103.1"
#define CLIENT_ID       "esp32_client"
#define QOS             0

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_topic_t _mqtt_topic;

static void *emcute_thread(void *arg) {
    (void) arg;
    emcute_run(EMCUTE_PORT, CLIENT_ID);
    return NULL;  // should never be reached
}

int main(void) {
    /* initialize the message queue */
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* connect to the broker */
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, MQ_SN_BROKER_IP, &gw.addr.ipv4);
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Could not connect to the broker.");
        return 1;
    }

    /* setup topic we want to publish to */
    _mqtt_topic.name = "state";
    if (emcute_reg(&_mqtt_topic) != EMCUTE_OK) {
        puts("Could not register topic.");
        return 1;
    }

    while (1) {
        const char *data = "work";
        if (emcute_pub(&_mqtt_topic, data, strlen(data), QOS) != EMCUTE_OK) {
            puts("Failed to publish data.");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);  // sleep for 5 seconds
    }

    /* this part will never be reached */
    emcute_discon();
    return 0;
}