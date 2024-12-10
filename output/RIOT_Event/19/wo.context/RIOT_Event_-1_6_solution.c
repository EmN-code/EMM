#include <stdio.h>
#include <stdint.h>
#include "xtimer.h"
#include "ztimer.h"
#include "event/periodic.h"
#include "event/callback.h"

#define INTERVAL_USEC (1U * 1000 * 1000) // 1 second interval in microseconds

static void timer_callback(void *arg)
{
    (void)arg; // Cast to void to suppress unused variable warning
    printf("Callback triggered\n");
}

int main(void)
{
    printf("Event periodic callback example\n");

    ztimer_t timer = {0};
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Initialize the timer using ztimer
    ztimer_set(&timer, INTERVAL_USEC);

    while (1) {
        // Wait for the timer to trigger
        xtimer_usleep(INTERVAL_USEC);
        // Simulate timer_callback manually as ztimer_set isn't auto-triggering without loop here
        timer_callback(NULL);
    }

    return 0;
}