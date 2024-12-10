
#include "xtimer.h"
#include "evtimer.h"
#include "msg.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U) // 1 second in microseconds

evtimer_t evtimer;
evtimer_msg_event_t event;

void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the event
    event.msg.sender_pid = thread_get_active()->pid;
    event.event.callback = periodic_callback;
    event.event.arg = NULL;

    // Add the event to the event timer
    evtimer_add_msg(&evtimer, &event, thread_get_active()->pid);

    // Set the timer to trigger the event every second
    xtimer_set(&evtimer.timer, TIMER_INTERVAL);

    // Main loop to keep the application running
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
