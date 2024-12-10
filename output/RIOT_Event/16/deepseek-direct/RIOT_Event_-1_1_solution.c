
#include "xtimer.h"
#include "periph/gpio.h"
#include "thread.h"
#include "event.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static event_t periodic_event;

void periodic_callback(void *arg) {
    (void)arg;  // Unused parameter

    // Example action: Toggle an LED
    static int led_state = 0;
    gpio_write(GPIO_PIN(0, 16), led_state);
    led_state = !led_state;

    // Reschedule the event
    event_post(EVENT_PRIO_MEDIUM, &periodic_event);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(GPIO_PIN(0, 16), GPIO_OUT);

    // Initialize the periodic event
    event_init(&periodic_event, periodic_callback, NULL);

    // Post the first event to start the periodic callback
    event_post(EVENT_PRIO_MEDIUM, &periodic_event);

    // Main loop to keep the application running
    while (1) {
        // The event system will handle the periodic callback
        xtimer_sleep(1);  // Sleep for 1 second to avoid busy-waiting
    }

    return 0;
}
