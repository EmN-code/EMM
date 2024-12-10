
#include <stdio.h>
#include <stdint.h>
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "ztimer.h"

#define INTERVAL (1000000U)  // 1 second interval
#define QUEUE_SIZE (8)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

static void periodic_callback(void *arg)
{
    msg_t msg;
    msg.content.value = (uint32_t)arg;
    mbox_put(&mbox, &msg);
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    ztimer_t timer;
    ztimer_periodic_callback_t periodic_event;

    ztimer_periodic_callback_init(&periodic_event, ZTIMER_USEC, &timer, &periodic_callback, (void *)INTERVAL);
    ztimer_periodic_callback_set_interval(&periodic_event, INTERVAL);
    ztimer_periodic_callback_start(&periodic_event);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received event with value: %lu\n", (unsigned long)msg.content.value);
    }

    return 0;
}
