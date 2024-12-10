#include "ztimer.h"
#include "xtimer.h"
#include "stdio.h"

void callback(void *arg) {
    printf("Callback triggered\n");
}

int main(void) {
    ztimer_t timer;
    timer.callback = callback;
    timer.arg = NULL;

    // Set the timer to trigger every 1000ms (1 second)
    uint32_t interval = ZTIMER_MSEC(1000);
    printf("Starting timer with interval: %lu ms\n", (unsigned long)interval);

    // Initialize and start the timer
    ztimer_set(&timer, true, interval);
    
    // Keep the program running indefinitely so the timer continues to trigger
    while (1) {
        ztimer_sleep(interval);
    }
}
