#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define TIMER_INTERVAL (1000000U)  // Timer interval in microseconds (1 second)
#define LED_PIN GPIO_PIN(PORT_A, 5) // Define the pin where the LED is connected

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED state
    printf("LED toggled\n");
}

int main(void) {
    // Initialize the GPIO for the LED
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error: failed to initialize LED_PIN\n");
        return 1;
    }

    // Initialize LED to be off initially
    gpio_clear(LED_PIN);

    // Setup the timer to trigger every 1 second
    static xtimer_t timer;
    xtimer_set(&timer, TIMER_INTERVAL);

    // Continuously reset the timer after each trigger
    while (1) {
        xtimer_usleep(TIMER_INTERVAL);
        timer_callback(NULL);
    }

    return 0;  // End of program
}