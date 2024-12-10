#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "msg.h"
#include "kernel.h"
#include "thread.h"
#include "evtimer.h"

#define QUEUE_SIZE 8
#define INTERVAL (1U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];

static msg_t queue[QUEUE_SIZE];
static mbox_t mbox;

void *event_loop(void *arg) {
    (void)arg;
    msg_t msg;
    evtimer_t evtimer;
    evtimer_msg_event_t event;

    evtimer_init_msg(&evtimer);
    evtimer_msg_event_init(&event, &msg, &mbox, EVENT_PRIO_HIGHEST);
    evtimer_add_msg(&evtimer, &event, INTERVAL);

    while (1) {
        if (mbox_get(&mbox, &msg, 1)) {
            printf("Received message: %lu\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    mbox_init(&mbox, queue, QUEUE_SIZE);

    if (thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                      THREAD_CREATE_STACKTEST, event_loop, NULL, "event_loop") <= KERNEL_PID_UNDEF) {
        puts("[Error] Failed to create thread!");
        return 1;
    }

    while (1) {
        msg_t msg;
        msg.content.value = random_uint32();
        mbox_put(&mbox, &msg);
        xtimer_sleep(2);
    }
    return 0;
}