#include <xtimer.h>
#include <stdio.h>
#include <event.h>

#define INTERVAL_SEC (1U)

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Callback function executed\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_init_msg(&evtimer);

    evtimer_msg_event_t event;
    msg_t msg;

    event.event.period = INTERVAL_SEC * US_PER_SEC;
    event.event.callback = periodic_callback;
    event.msg = msg;

    evtimer_add_msg(&evtimer, &event, thread_getpid());

    while(1) {
        xtimer_sleep(INTERVAL_SEC);
    }
    return 0;
}