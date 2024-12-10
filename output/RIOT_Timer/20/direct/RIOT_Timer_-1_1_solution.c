#include <ztimer/ztimer.h>
#include <periph/gpio.h>

// Define the GPIO pin where the LED is connected
#define LED_PIN GPIO_PIN(0, 2)

// Timer callback function
void timer_callback(void *arg) {
    (void)arg;  // Unused parameter
    // Toggle the LED state
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Define a ztimer object with 1-second periodic timeout
    ztimer_t timer;
    ztimer_set(ZTIMER_MSEC, &timer, 1000, timer_callback, NULL);

    while (1) {
        // Pause the thread so that it waits for interrupts to trigger
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}