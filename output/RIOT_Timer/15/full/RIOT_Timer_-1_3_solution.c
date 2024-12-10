#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define TIMER_INTERVAL (1U * US_PER_SEC)  // Timer interval set to 1 second in microseconds

static void timer_callback(void *arg) {
    (void)arg;
    printf("LED toggled\n");
    // Simulate LED toggle
}

int main(void) {
    evtimer_t evtimer;
    evtimer_msg_event_t event;
    mbox_t mbox;

    // Initialize message queue for the timer
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    // Initialize evtimer with message event
    evtimer_init_msg(&evtimer);
    evtimer_callback_t callback = timer_callback;

    // Set up the event for the evtimer
    event.msg.type = 0xABCD; // message type
    event.msg.content.ptr = NULL; // content
    evtimer_add_msg(&evtimer, &event, KERNEL_PID_UNDEF);

    // Initialize mbox for evtimer
    evtimer_init_mbox(&evtimer);
    evtimer_add_mbox(&evtimer, &event, &mbox);

    // Start timer periodically
    ztimer_periodic_wakeup(ZTIMER_SEC, TIMER_INTERVAL);

    while (1) {
        msg_receive(&msg_queue[0]);
        evtimer_geral(&mbox);
    }

    return 0;
}