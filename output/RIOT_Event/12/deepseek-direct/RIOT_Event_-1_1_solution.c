
#include "xtimer.h"
#include "periph/gpio.h"
#include "event.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static event_t periodic_event;

void periodic_callback(void *arg) {
    (void)arg;  // Unused parameter

    // Example action: Toggle an LED
    gpio_toggle(GPIO_PIN(0, 16));  // Assuming GPIO_PIN(0, 16) is connected to an LED

    // Reschedule the event
    event_post(EVENT_PRIO_MEDIUM, &periodic_event);
}

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(GPIO_PIN(0, 16), GPIO_OUT);

    // Initialize the periodic event
    periodic_event.handler = periodic_callback;

    // Post the event for the first time
    event_post(EVENT_PRIO_MEDIUM, &periodic_event);

    // Main loop
    while (1) {
        // The main loop can be used for other tasks or left empty
        xtimer_sleep(1);  // Sleep for 1 second to avoid busy-waiting
    }

    return 0;
}
