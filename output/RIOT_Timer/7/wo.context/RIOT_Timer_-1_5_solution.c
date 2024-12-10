#include <xtimer.h>
#include <periph/gpio.h>
#include <stdio.h>

#define LED_PIN GPIO_PIN(0, 2)  // Define the LED pin, ensure this matches your actual setup
#define TIMER_INTERVAL (1U * 1000000U) // 1 second interval

static void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED state
    puts("LED toggled");
}

int main(void) {
    // Initialize LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Error: unable to initialize the LED pin");
        return 1;
    }

    // Declare and initialize a timer
    xtimer_t timer;
    xtimer_set_callback(&timer, timer_callback);  
    
    // Set the timer to the defined interval (1 second)
    xtimer_set(&timer, TIMER_INTERVAL);
    puts("Timer initialized");

    // Main loop
    while (1) {
        xtimer_sleep(1);  // Sleep to prevent busy waiting
    }
    return 0;
}