
#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "event.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static event_t periodic_event;

void periodic_callback(void *arg) {
    (void)arg;  // Unused parameter

    // This function will be called every second
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize the periodic event
    event_init(&periodic_event, periodic_callback, NULL);

    // Start the periodic event
    while (1) {
        xtimer_sleep(1);  // Sleep for 1 second
        event_post(&periodic_event, NULL);
    }

    return 0;
}
