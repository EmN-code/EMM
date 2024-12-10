#include <stdint.h>
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN     GPIO_PIN(PORT_A, 5) // Define the GPIO pin where the LED is connected
#define INTERVAL    (1000000)  // Interval for the timer in microseconds, 1 second

static void timer_callback(void *arg) {
    (void)arg;
    static int led_state = 0;
    led_state = !led_state;  // Toggle LED state
    gpio_write(LED_PIN, led_state);
    printf("LED is %s\n", led_state ? "ON" : "OFF");
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Start the timer loop
    while (1) {
        // Wait for the specified interval
        xtimer_usleep(INTERVAL);
        // Execute the timer callback
        timer_callback(NULL);
    }

    return 0;
}