#include "emcute.h"
#include "net/emcute.h"
#include "thread.h"
#include "ztimer.h"
#include "msg.h"

#include <stdio.h>
#include <string.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "device/state"
#define PUB_INTERVAL (5U * ZTIMER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    puts("Message published");
}

static void *thread_handler(void *arg) {
    (void)arg;
    evtimer_t evtimer;
    evtimer_msg_event_t event;
    msg_t msg;
    int count = 0;
    char pubdata[32];

    evtimer_init_msg(&evtimer);
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    while (1) {
        snprintf(pubdata, sizeof(pubdata), "state: %s", count % 2 == 0 ? "work" : "not work");

        emcute_topic_t t;
        t.name = TOPIC_NAME;
        emcute_reg(&t);

        if (emcute_pub(&t, pubdata, strlen(pubdata), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Publish failed");
            continue;
        }

        puts("Published state");
        ++count;

        /* Sleep the thread for a specified interval */
        ztimer_sleep(ZTIMER_SEC, PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    puts("MQTT-SN Publisher");

    /* create the thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_handler, NULL, "mqtt_pub");

    /* create a message queue for handling the evtimer events */
    msg_init_queue(queue, sizeof(queue)/sizeof(msg_t));

    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &(gw.addr.ipv4));

    emcute_con(&gw, false, NULL, NULL, 0, 0);

    return 0;
}