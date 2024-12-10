
#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "event.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static event_t periodic_event;

void periodic_callback(void *arg) {
    (void)arg;  // Unused parameter

    // Example action: Toggle a GPIO pin
    gpio_toggle(GPIO_PIN(0, 10));  // Toggle GPIO0_10

    // Reschedule the event
    event_post(EVENT_PRIO_MEDIUM, &periodic_event);
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(GPIO_PIN(0, 10), GPIO_OUT);

    // Initialize the periodic event
    event_init(&periodic_event, periodic_callback, NULL);

    // Post the first event to start the periodic callback
    event_post(EVENT_PRIO_MEDIUM, &periodic_event);

    // Main loop to keep the application running
    while (1) {
        xtimer_sleep(PERIODIC_INTERVAL);
    }

    return 0;
}
