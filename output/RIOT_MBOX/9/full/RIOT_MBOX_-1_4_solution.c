#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "string.h"
#include "stdbool.h"
#include "stdio.h"
#include "netinet/in.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define MSG_QUEUE_SIZE (8U)
#define SENDER_PRIO   (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

static mbox_t mbox;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        // Send a message every second
        msg.content.value = xtimer_usec_from_ticks(xtimer_now());
        mbox_put(&mbox, &msg);
        xtimer_sleep(1);
    }

    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        // Receive messages indefinitely
        if (_mbox_get(&mbox, &msg, 1)) {
            // Print the value received
            printf("Received message: %lu\n", msg.content.value);
        }
    }

    return NULL;
}

int main(void) {
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO,
                  THREAD_CREATE_STACKTEST, sender, NULL, "sender");

    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO,
                  THREAD_CREATE_STACKTEST, receiver, NULL, "receiver");

    // Keep the main function running
    while(1) {
        xtimer_sleep(1000);
    }

    return 0;
}
