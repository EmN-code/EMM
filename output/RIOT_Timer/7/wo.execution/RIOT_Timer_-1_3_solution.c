#include <xtimer.h>         // Necessary for xtimer functions
#include <periph/gpio.h>    // Necessary for GPIO operations

// Define the LED pin (adjust based on your ESP32 configuration)
#define LED_PIN GPIO_PIN(PORT_A, 2)

// Callback function for the timer
void led_toggle(void *arg) {
    (void)arg;
    // Toggle the LED state
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize the timer
    xtimer_ticks32_t interval = xtimer_usec_from_ticks(1000000); // 1 second intervals
    
    // Continuously toggle the LED every second
    while (1) {
        led_toggle(NULL);
        xtimer_sleep(interval); // Sleep for the defined interval
    }

    return 0;
}