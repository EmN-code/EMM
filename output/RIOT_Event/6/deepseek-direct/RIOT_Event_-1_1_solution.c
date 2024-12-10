
#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "event.h"

#define PERIOD_MS (1000U)  // 1 second period

static event_t periodic_event;

void periodic_callback(void *arg) {
    (void)arg;  // Unused parameter

    // This function will be called every second
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize the periodic event
    event_init(&periodic_event, periodic_callback, NULL);

    // Start the periodic event with a 1 second period
    xtimer_periodic_event(&periodic_event, PERIOD_MS * 1000);

    // Main loop (optional, depending on your application)
    while (1) {
        // You can add additional logic here if needed
        xtimer_sleep(1);  // Sleep for 1 second to avoid busy-waiting
    }

    return 0;
}
