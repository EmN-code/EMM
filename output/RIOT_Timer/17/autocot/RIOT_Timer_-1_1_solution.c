#include <tim.h>
#include <timer.h>
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_B, 0)  // Assume LED is connected to pin B0
#define TIMER_FREQUENCY_HZ 1  // 1 Hz timer frequency for 1 second interval

static void timer_callback(void *arg) {
    (void)arg;  // Unused parameter
    static bool led_state = false;  // Initialize LED state variable
    
    if (led_state) {
        gpio_clear(LED_PIN);  // Turn off LED
    } else {
        gpio_set(LED_PIN);  // Turn on LED
    }
    
    led_state = !led_state;  // Toggle LED state
}

int main(void) {
    // Initialize GPIO pin for the LED
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Error initializing the LED pin");
        return 1;
    }
    
    // Timer setup
    tim_t dev = 0; // Using timer 0 as an example
    uint32_t timer_freq = TIMER_FREQUENCY_HZ;
    if (timer_init(dev, timer_freq, timer_callback, NULL)) {
        printf("Failed to initialize timer\n");
        return 1;
    }
    
    // Start the timer
    timer_start(dev);
    puts("Timer started, LED will toggle every second.");
    
    // Keep the main function running
    while (true) {
        xtimer_sleep(1);
    }

    return 0;
}