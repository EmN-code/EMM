#include <xtimer.h>      // Timer related functions
#include <thread.h>     // Thread related functions
#include <stdio.h>      // Standard input/output
#include <periph/gpio.h> // Peripheral GPIO functions

#define LED_PIN (GPIO_PIN(PORT_A, 8)) // Define the LED pin

void *led_blink(void *arg) {
    (void)arg; // Unused
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        gpio_toggle(LED_PIN);
        xtimer_sleep(1); // Sleep for 1 second
    }

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, led_blink, NULL, "led_blink_thread");
    
    // The main thread needs to do some work to keep the program running
    while(1) {
        thread_yield(); // Yield control to other threads
    }
    return 0;
}
