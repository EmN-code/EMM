#include <xtimer.h>
#include <timex.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_B, 0) // Adjust this to the correct port and pin

tim_t dev;

timer_cb_t timer_callback(void *arg) {
    (void)arg;
    // Toggle the state of the LED
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    timer_init(dev, 1, timer_callback, NULL);

    while (1) {
        // Sleep for 1 second
        xtimer_sleep(1);
    }

    return 0;
}