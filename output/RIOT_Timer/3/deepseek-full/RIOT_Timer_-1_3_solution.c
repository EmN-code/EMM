
#include <stdio.h>
#include <stddef.h>
#include "ztimer.h"
#include "periph/gpio.h"
#include "event/periodic_callback.h"

#define LED_PIN GPIO_PIN(0, 13)  // Adjust the GPIO pin according to your ESP32 setup

static void led_toggle_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize the LED pin as output
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        puts("Failed to initialize LED pin");
        return 1;
    }

    // Create and start a periodic callback event
    event_periodic_callback_t event;
    event_periodic_callback_create(&event, ZTIMER_MSEC, 1000, NULL, led_toggle_callback, NULL);

    // Main loop
    while (1) {
        // The periodic callback will handle the LED toggling
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}
