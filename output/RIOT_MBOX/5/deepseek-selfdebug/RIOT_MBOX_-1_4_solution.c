
#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "ztimer.h"
#include "event.h"

#define MSG_QUEUE_SIZE (16)
#define INTERVAL (1000000U)  // 1 second in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void event_handler(void *arg)
{
    (void)arg;
    puts("Event triggered!");
}

static void timer_callback(void *arg)
{
    msg_t msg;
    msg.content.ptr = NULL;
    mbox_put(&mbox, &msg);
}

int main(void)
{
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    event_t event;
    event.handler = (event_handler_t)event_handler;

    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    while (1) {
        ztimer_set(ZTIMER_USEC, &timer, INTERVAL);
        msg_t msg;
        mbox_get(&mbox, &msg, 1);
        event_post(EVENT_PRIO_LOWEST, &event);
    }

    return 0;
}
