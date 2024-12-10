#include <ztimer.h>
#include <stdio.h>
#include <evtimer.h>
#include <event/thread.h>

#define TIMER_INTERVAL (ZTIMER_MSEC(1000))

static evtimer_t periodic_timer;
event_queue_t event_queue;

void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered\n");
}

int main(void) {
    // Initialize an event queue
    event_queue_init(&event_queue);

    // Initialize an event timer
    evtimer_init(&periodic_timer, &event_queue, periodic_callback, NULL);

    // Set the timer to trigger every second
    evtimer_set(&periodic_timer, TIMER_INTERVAL, EVTFLAG_PERIODIC);

    // Start processing events
    event_thread_start(&event_queue, EVENT_QUEUE_PRIO_MEDIUM);

    return 0;
}