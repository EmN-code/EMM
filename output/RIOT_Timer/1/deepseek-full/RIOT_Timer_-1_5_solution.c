
#include "kernel_defines.h"
#include "ztimer.h"
#include "periph/gpio.h"
#include "event/periodic_callback.h"
#include "assert.h"
#include "thread.h"

#define LED_PIN GPIO_PIN(0, 13)  // Example LED pin for ESP32

static void led_toggle_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize the LED pin as output
    assert(gpio_init(LED_PIN, GPIO_OUT) == 0);

    // Create and start a periodic callback event
    event_periodic_callback_t event;
    event_periodic_callback_create(&event, ZTIMER_MSEC, 1000, NULL, led_toggle_callback, NULL);

    // Main loop to keep the thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}
