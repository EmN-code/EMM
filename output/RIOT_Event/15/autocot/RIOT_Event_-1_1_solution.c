#include <thread.h>
#include <event/periodic.h>
#include <event/callback.h>

void callback(void *arg) {
    // Action to perform every second
    printf("Periodic event triggered!\n");
}

int main(void) {
    msg_init_queue(msg_q, sizeof(msg_q) / sizeof(msg_t));

    // Define periodic event
    evtimer_t evtimer;
    evtimer_init_msg(&evtimer);

    evtimer_periodic_event_t period_event;
    evtimer_periodic_callback_init(&period_event, &evtimer, 1 * US_PER_SEC, callback, NULL);

    // Add the periodic event
    evtimer_add_periodic(&evtimer, &period_event);

    // Start the event handler loop (blocks forever)
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}