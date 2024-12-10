#include <ztimer.h>
#include <stdio.h>
#include <stdint.h>

#define TIMER_INTERVAL_SEC (1U)

static ztimer_t timer;

void timer_callback(void *arg)
{
    (void)arg; // Unused argument
    printf("Timer callback function executed.\n");

    // Re-arm the timer for continuous execution every second
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL_SEC * 1000);
}

int main(void)
{
    printf("Initializing timer...\n");
    
    // Initialize timer
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer for the first time
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL_SEC * 1000);

    while (1) {
        // Main loop doing other tasks if necessary
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL_SEC * 1000);
    }
    return 0;
}