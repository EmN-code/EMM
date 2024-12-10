#include "timers.h"
#include "thread.h"
#include "periph/gpio.h"
#include <stdio.h>

#define LED_PIN GPIO_PIN(0, 2) // assuming the LED is connected to GPIO pin 2 on port 0

static void timer_callback(void *arg) {
    static bool led_status = false;
    
    // Toggle the LED status
    led_status = !led_status;
    gpio_write(LED_PIN, led_status);
    
    printf("LED is %s\n", led_status ? "ON" : "OFF");
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Configure the timer
    timer_init(TIMER_DEV(0), 1000, timer_callback, NULL);
    
    // Start the timer
    timer_start(TIMER_DEV(0));
    
    // Keep the main function alive
    while (1) {
        thread_yield();
    }
  
    return 0;
}