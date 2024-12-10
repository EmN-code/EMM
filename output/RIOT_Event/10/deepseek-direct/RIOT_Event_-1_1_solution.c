
#include "xtimer.h"
#include "periph/gpio.h"
#include "event.h"
#include "thread.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static event_t periodic_event;

void periodic_callback(void *arg) {
    (void)arg;  // Unused parameter

    // Example action: Toggle a GPIO pin
    gpio_toggle(GPIO_PIN(0, 13));  // Toggle GPIO0_13

    // Reschedule the event
    event_post(EVENT_PRIO_MEDIUM, &periodic_event);
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(GPIO_PIN(0, 13), GPIO_OUT);

    // Initialize the periodic event
    periodic_event.handler = periodic_callback;

    // Post the first event to start the periodic callback
    event_post(EVENT_PRIO_MEDIUM, &periodic_event);

    // Main loop (optional, depending on your application)
    while (1) {
        // Your main application logic can go here
        xtimer_sleep(1);  // Sleep for 1 second
    }

    return 0;
}
