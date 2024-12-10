
#include "periph/gpio.h"
#include "event/periodic_callback.h"
#include "ztimer.h"

#define LED0_PIN GPIO_PIN(0, 16)  // Example GPIO pin for LED on ESP32

static void led_toggle_callback(void *arg)
{
    (void)arg;  // Unused argument
    gpio_toggle(LED0_PIN);
}

int main(void)
{
    // Initialize the LED pin as output
    gpio_init(LED0_PIN, GPIO_OUT);

    // Create and start the periodic callback event
    event_periodic_callback_t event;
    event_periodic_callback_create(&event, ZTIMER_MSEC, 1000, NULL, led_toggle_callback, NULL);

    return 0;
}
