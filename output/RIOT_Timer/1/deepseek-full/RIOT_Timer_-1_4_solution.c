
#include "kernel_defines.h"
#include "ztimer.h"
#include "periph/gpio.h"
#include "event/periodic_callback.h"
#include "assert.h"

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO 13 on port 0 for the LED

static void led_toggle(void) {
    gpio_toggle(LED_PIN);
}

static void timer_callback(void *arg) {
    (void)arg;
    led_toggle();
}

int main(void) {
    // Initialize the LED pin as output
    assert(gpio_init(LED_PIN, GPIO_OUT) == 0);

    // Set up a periodic callback to toggle the LED every second
    event_periodic_callback_t periodic_cb;
    event_periodic_callback_init(&periodic_cb, ZTIMER_MSEC, timer_callback, NULL);
    event_periodic_callback_start(&periodic_cb, 1000 * US_PER_MS);  // 1 second interval

    // Keep the main thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}
