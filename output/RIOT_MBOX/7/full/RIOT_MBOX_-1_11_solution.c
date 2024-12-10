#include "xtimer.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "string.h"
#include "stddef.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "event/thread.h"
#include "thread.h"
#include "timex.h"
#include "ztimer.h"
#include "time_units.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "mutex.h"

#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MSG_QUEUE_SIZE  (8)
#define INTERVAL        (1000000U)  // 1 second in microseconds

static char thread_stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

void *event_thread(void *arg) {
    (void)arg;
    evtimer_mbox_event_t event;
    msg_t msg;

    while (1) {
        /* Wait for the next event */
        if (_mbox_get(&mbox, &msg, 1)) {
            printf("Received message with value: %d\n", msg.content.value);
        }
    }

    return NULL;
}

int main(void) {
    /* Initialize the message queue and mailbox */
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);
    /* Initialize the event timer for the mailbox */
    evtimer_init_mbox(&evtimer);

    /* Create the event handling thread */
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY, 0,
                  event_thread, NULL, "event_thread");

    /* Send events at intervals */
    msg_t msg;
    msg.content.value = 42;  // Example message content

    while (1) {
        evtimer_add_mbox(&evtimer, &event, &mbox);
        evtimer_set(&evtimer, INTERVAL);

        _mbox_get(&mbox, &msg, 1);
        xtimer_sleep(1);
    }

    return 0;
}