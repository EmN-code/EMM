#include <stdio.h>
#include <event.h>
#include <xtimer.h>

#define PERIOD 1U

static void event_periodic_handler(void *arg) {
    (void)arg;
    printf("Event triggered\n");
}

int main(void) {
    event_t periodic_event;
    periodic_event.handler = event_periodic_handler;
    periodic_event.event_type = EVENT_TYPE_TIMER;
    periodic_event.event_timer.interval = PERIOD;
    periodic_event.handler_ctx = NULL;

    printf("Starting event loop\n");

    while (1) {
        event_post(&periodic_event);
        xtimer_sleep(PERIOD);
    }

    return 0;
}