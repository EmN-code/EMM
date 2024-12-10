#include "ztimer.h"
#include "timex.h"
#include "thread.h"
#include "stdio.h"

#define INTERVAL (1U * US_PER_SEC)  // Set interval of 1 second

void callback_function(void *arg) {
    (void)arg;  // Suppress unused variable warning
    puts("Callback function called!");
}

int main(void) {
    ztimer_t timer;
    timer.callback = callback_function;
    timer.arg = NULL;

    // Initialize the timer
    xtimer_set(&timer, INTERVAL);

    while (1) {
        // Main loop does nothing, just waits for the callback to fire.
        thread_yield();
    }
    return 0;
}