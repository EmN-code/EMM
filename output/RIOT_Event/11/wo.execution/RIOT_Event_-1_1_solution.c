#include <timex.h>
#include <thread.h>
#include <event/periodic.h>
#include <event/callback.h>

#define EVENT_INTERVAL (1U * US_PER_SEC)  // 1 second interval

static void _timer_callback(void *arg) {
    (void)arg;
    printf("Event triggered every second!\n");
}

int main(void) {
    evtimer_t evtimer;

    // Initialize the event timer
    evtimer_init(&evtimer);

    // Create a periodic event
    evtimer_event_t event;
    evtimer_set_periodic(&event, EVENT_INTERVAL, _timer_callback);

    // Add the periodic event to the timer
    evtimer_add(&evtimer, &event);

    // Let the thread run indefinitely
    while (1) {
        evtimer_update(&evtimer);
        thread_sleep();
    }

    return 0;
}