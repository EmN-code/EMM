#include "timers/tim.h"
#include "gpio.h"
#include "thread.h"

#define LED_PIN GPIO_PIN(PORT_A, 5)  // Assuming LED is connected to PORT_A pin 5.

void timer_callback(void *arg) {
    (void)arg;  // Suppress unused parameter warning
    gpio_toggle(LED_PIN);  // Toggle the LED state
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Configure the timer
    tim_t timer = TIM_DEV(0);  // Use the first timer available
    uint32_t freq = 1;  // 1 Hz frequency for 1-second interval

    // Initialize the timer with the specified frequency and callback
    if (timer_init(timer, freq, timer_callback, NULL) < 0) {
        // Initialization failed
        return 1;
    }

    // Start the timer
    timer_start(timer);

    // Main loop
    while (1) {
        // Sleep the thread to save CPU cycles
        thread_sleep();
    }

    return 0;
}