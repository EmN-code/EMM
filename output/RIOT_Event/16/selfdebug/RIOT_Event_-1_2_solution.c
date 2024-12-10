#include <thread.h>
#include <event/periodic.h>
#include <stdio.h>

static void event_callback(void *arg) {
    (void)arg;
    printf("Event triggered\n");
}

int main(void) {
    evtimer_t ev;
    evtimer_event_t event;

    ztimer_periodic_init(ZTIMER_MSEC, &event.event, event_callback, NULL, 1000);
    evtimer_init(&ev, event_callback, NULL);
    evtimer_add(&ev, &event);

    while(1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}