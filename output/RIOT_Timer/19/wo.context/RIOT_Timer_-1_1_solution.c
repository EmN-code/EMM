#include <timex.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_B, 0) // Define the pin for the LED

void timer_callback(void *arg) {
    (void)arg;
    static bool led_state = false;
    
    // Toggle the LED state
    led_state = !led_state;
    gpio_write(LED_PIN, led_state);
    printf("LED is now: %s\n", led_state ? "ON" : "OFF");
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    printf("Timer example starting.\n");
    
    // Initialize the timer
    timex_t interval = {1, 0}; // 1 second interval
    timer_t timer;
    timer_init(&timer, TIM_DEV(0), timer_callback, NULL);
    
    // Start the timer
    timer_set(&timer, interval);
    timer_start(&timer);
    
    // Main loop
    while (1) {
        // Sleep to keep the CPU free for the timer interrupt
        xtimer_sleep(1);
    }
    return 0;
}