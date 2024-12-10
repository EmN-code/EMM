// Include the necessary RIOT header files in descending alphabetical order
#include <xtimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 5) // Assuming the LED is connected to PA5

// Callback function to toggle the LED state
void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Define a timer
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Run the timer every second
    xtimer_set(&timer, XTIMER_SEC(1));

    // Keep the main thread running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}