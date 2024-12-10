
#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"

#define PERIOD_MS (1000U)  // 1 second period

static void event_periodic(void) {
    printf("Event triggered!\n");
}

int main(void) {
    // Initialize the xtimer module
    xtimer_init();

    // Set up a periodic timer to call event_periodic every PERIOD_MS milliseconds
    xtimer_t timer;
    xtimer_periodic_callback(&timer, PERIOD_MS * 1000, (xtimer_callback_t)event_periodic);

    // Main loop to keep the program running
    while (1) {
        // The main thread can do other tasks here if needed
        xtimer_sleep(1);  // Sleep for 1 second to avoid busy-waiting
    }

    return 0;
}
