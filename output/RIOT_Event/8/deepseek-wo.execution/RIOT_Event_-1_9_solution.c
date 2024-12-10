
#include <stdio.h>
#include <xtimer.h>

// Define a callback function that will be called every second
void event_periodic_callback(void) {
    printf("Event periodic callback triggered!\n");
}

int main(void) {
    // Initialize the xtimer
    xtimer_init();

    // Set up a periodic timer to call the callback function every second
    xtimer_t timer;
    uint32_t period = 1000000; // 1 second in microseconds
    xtimer_periodic_wakeup(&timer, period);

    while (1) {
        // Wait for the next timer event
        xtimer_periodic_wakeup(&timer, period);
        event_periodic_callback();
    }

    return 0;
}
