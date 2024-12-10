
#include <stdio.h>
#include "periph/gpio.h"
#include "ztimer.h"
#include "event/periodic_callback.h"

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO0, pin 13 for the LED

static void led_toggle_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize the LED pin as output
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        puts("Failed to initialize GPIO");
        return 1;
    }

    // Create an event queue
    event_queue_t event_queue;
    event_queue_init(&event_queue);

    // Create a periodic callback event
    event_periodic_callback_t periodic_event;
    event_periodic_callback_create(&periodic_event, ZTIMER_MSEC, 1000, &event_queue, led_toggle_callback);

    // Start the periodic event
    event_periodic_callback_start(&periodic_event);

    // Run the event loop
    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
