#include <timex.h>
#include <ztimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2) // Assume LED is connected to pin 0.2

static void timer_callback(void *arg) {
    (void)arg;
    // Toggle the LED state
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin for the LED to output mode
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Define a timer with a 1-second interval
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    timer.min_interval = US_PER_SEC; // 1 second

    // Set the timer
    ztimer_set(ZTIMER_SEC, &timer, 1);
    
    // Run the main program loop which does not do much in this case
    while (1) {
        // Idle loop to keep the program running
    }

    return 0;
}