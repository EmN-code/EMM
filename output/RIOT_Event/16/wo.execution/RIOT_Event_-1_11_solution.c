#include <xtimer.h>
#include <stdio.h>
#include <thread.h>
#include <evtimer_msg.h>
#include <event/callback.h>
#include <msg.h>
#include <stdlib.h>

#define INTERVAL (1U * US_PER_SEC)

static evtimer_msg_t evtimer;
static evtimer_msg_event_t event;

void cb(void *arg) {
    (void)arg;
    printf("Timer event triggered\n");
}

int main(void) {
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));
    evtimer_init_msg(&evtimer);
    evtimer_msg_event_callback_init(&event, cb, NULL);
    evtimer_add_msg(&evtimer, &event.super, thread_getpid());

    while (1) {
        evtimer_run(&evtimer);

        xtimer_usleep(INTERVAL);
    }
    return 0;
}