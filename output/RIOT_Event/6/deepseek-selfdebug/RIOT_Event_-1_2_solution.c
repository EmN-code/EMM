
#include "xtimer.h"
#include "evtimer.h"
#include "evtimer_msg.h"
#include "thread.h"
#include "msg.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static evtimer_t periodic_timer;
static evtimer_msg_event_t periodic_event;

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize the event timer
    evtimer_init(&periodic_timer);

    // Initialize the event
    evtimer_event_init(&periodic_event.event, periodic_callback, NULL);

    // Add the event to the timer with a periodic interval
    evtimer_add_event(&periodic_timer, &periodic_event.event, PERIODIC_INTERVAL);

    // Main loop to keep the application running
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
