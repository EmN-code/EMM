#include <ztimer.h>
#include <stdio.h>

static ztimer_t timer;

void event_periodic_cb(void *arg) {
    puts("Periodic event triggered.");
    /* Re-set the callback for the next second */
    ztimer_set_msg(ZTIMER_MSEC, &timer, 1000, NULL, NULL);
}

int main(void) {
    puts("RIOT event_periodic example");
    
    /* Initialize the ztimer_t struct */
    ztimer_set(ZTIMER_MSEC, &timer, 1000);
    timer.callback = event_periodic_cb;
    timer.arg = NULL;
    
    /* Start the timer */
    ztimer_set_msg(ZTIMER_MSEC, &timer, 1000, NULL, NULL);
    
    while (1) {
        /* Simulate doing other tasks */
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return 0;
}
