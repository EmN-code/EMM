#include "xtimer.h"
#include "ztimer.h"
#include "thread.h"
#include "msg.h"
#include "event.h"
#include "event/callback.h"
#include <stdio.h>  

#define INTERVAL (1000000U) // 1 second interval
#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)

static void event_callback(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    puts("Event triggered!");
}

int main(void) {
    msg_t queue[8];
    mbox_t mbox;
    mbox_init(&mbox, queue, 8);

    char stack[THREAD_STACKSIZE_MAIN];
    event_queue_t queue;
    event_t event;
    event.callback = event_callback;

    msg_init_queue(queue, sizeof(queue) / sizeof(queue[0]));

    ztimer_clock_t *clock = ZTIMER_MSEC;
    ztimer_sleep(clock, INTERVAL);

    evtimer_t evtimer;
    evtimer_init_msg(&evtimer, clock, &mbox); 
    evtimer_add(&evtimer, &event, INTERVAL);

    while (1) {
        msg_t msg;
        if (ztimer_msg_receive(&mbox, &msg)) {
            callback(event.callback, NULL);
        }
    }
    return 0;
}