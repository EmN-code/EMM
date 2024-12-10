#include <ztimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2)  // Example Pin, change as per your board

void led_toggle(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Create the ztimer
    ztimer_t timer;
    ztimer_set(&timer, ZTIMER_MSEC, 1000, led_toggle, NULL);
    ztimer_start(&timer);

    // Main loop
    while (1) {
        // toggle LED every second via ztimer callback
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;  // Should never be reached
}