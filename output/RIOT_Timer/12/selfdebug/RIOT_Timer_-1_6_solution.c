#include <xtimer.h>  // For xtimer functions
#include <timex.h>  // For time functions and structures
#include <stdio.h>  // For standard input/output functions
#include <periph/gpio.h>  // For GPIO functions

// Define the LED pin. Adjust the port and pin as necessary for your setup.
#define LED_PIN GPIO_PIN(PORT_A, 0)

// Timer interval in microseconds (1 second)
#define INTERVAL (1U * US_PER_SEC)

// Timer callback function
void timer_callback(void *arg) {
    (void)arg;
    // Toggle the LED
    gpio_toggle(LED_PIN);
    printf("LED toggled.\n");
}

int main(void) {
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing GPIO\n");
        return 1;
    }

    // Initialize the timer
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer to trigger every second
    xtimer_set(&timer, INTERVAL);

    // Keep the main function running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}