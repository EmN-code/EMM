
#include <stdlib.h>
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <unistd.h>

#define TIMER_INTERVAL (1 * 1000 * 1000U) // 1 second in microseconds

void event_periodic_callback(void) {
    printf("Event periodic callback triggered!\n");
}

int main(void) {
    xtimer_t timer;

    // Initialize the xtimer
    xtimer_init();

    // Set the periodic callback to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL, (void (*)(void *))event_periodic_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        thread_yield();
    }

    return 0;
}
