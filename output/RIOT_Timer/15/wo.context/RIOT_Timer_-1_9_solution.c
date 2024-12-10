#include <xtimer.h>  // For xtimer_sleep
#include <timex.h>   // For timex_get
#include "periph/gpio.h" // For GPIO control

#define TIMER_INTERVAL (1U * US_PER_SEC) // Defines a one-second interval

// Function to be called by the timer every second
timer_t timer;

void on_timer(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    // Toggle LED
    gpio_toggle(GPIO_PIN(PORT_A, 5)); // Assuming LED is connected to PORT_A, Pin 5
}

int main(void) {
    // Initialize GPIO Pin
    if (gpio_init(GPIO_PIN(PORT_A, 5), GPIO_OUT)) {
        puts("Error initializing GPIO");
        return 1;
    }

    // Set up the timer
    timer_init(&timer, TIMER_INTERVAL, on_timer, NULL);

    // Start the timer
    if(!timer_set(&timer, TIMER_INTERVAL)) {
        puts("Error setting timer");
        return 1;
    }

    // Let the main function keep running to keep the timer active
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}