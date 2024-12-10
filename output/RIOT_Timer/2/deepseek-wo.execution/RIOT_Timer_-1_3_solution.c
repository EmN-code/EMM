
#include "xtimer.h"
#include "periph/gpio.h"
#include "event/periodic_callback.h"

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO0_13 is the LED pin on ESP32

static void led_toggle_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create and start the periodic callback event
    event_periodic_callback_t event;
    event_periodic_callback_create(&event, ZTIMER_MSEC, 1000, NULL, led_toggle_callback, NULL);

    // Start the periodic callback
    event_periodic_callback_start(&event);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
