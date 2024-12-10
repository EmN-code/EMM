#include <ztimer.h> // Include ztimer header for timer functions
#include <stdio.h>

static ztimer_periodic_t periodic_timer;

// This is the callback function that will be called every second
static void callback(void *arg) {
    puts("Callback called!");
}

int main(void) {
    ztimer_clock_t *clock = ZTIMER_USEC;
    ztimer_periodic_init(clock, &periodic_timer, 1*1000000, callback, NULL);
    ztimer_periodic_start(&periodic_timer);

    // Keep the program running to observe the callback
    while(1);
    return 0;
}