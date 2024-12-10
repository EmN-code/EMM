
#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "event.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static event_t periodic_event;

void periodic_callback(void *arg) {
    (void)arg;  // Unused parameter

    // Example action: Toggle a GPIO pin
    gpio_toggle(GPIO_PIN(0, 13));  // Toggle GPIO0_13

    // Reschedule the event
    event_post(&periodic_event, periodic_callback, NULL);
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(GPIO_PIN(0, 13), GPIO_OUT);

    // Initialize the event
    event_init(&periodic_event, NULL, NULL);

    // Start the periodic event
    event_post(&periodic_event, periodic_callback, NULL);

    // Main loop to keep the application running
    while (1) {
        xtimer_sleep(1);  // Sleep for 1 second to keep the main thread alive
    }

    return 0;
}
