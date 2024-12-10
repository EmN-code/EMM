#include <xtimer.h> // For the timer functions
#include <periph/gpio.h> // For controlling the GPIO (LED)

#define LED_PIN GPIO_PIN(PORT_A, 0) // Define the LED pin. Make sure PORT_A and the pin number corresponds to your setup.
#define TIMER_INTERVAL (1000000U) // 1 second interval in microseconds

void timer_callback(void *arg) {
    (void)arg; // Just to suppress unused warning
    gpio_toggle(LED_PIN); // Toggle the LED state
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Timer setup
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set and start the timer
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        // Sleep in main loop to save power
        xtimer_sleep(1);
    }

    return 0;
}
