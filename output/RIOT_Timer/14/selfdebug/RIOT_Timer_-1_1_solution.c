#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PORT_A, 5) // Assuming LED is connected to GPIO Port A, Pin 5
#define INTERVAL (1U * US_PER_SEC) // 1 second interval

void *led_blinker(void *arg) {
    (void)arg;
    while (1) {
        gpio_toggle(LED_PIN); // Toggle the LED state
        xtimer_sleep(1); // Wait for 1 second
    }
    return NULL;
}

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a new thread for the LED blinking task
    thread_create(
        NULL,       // Allocate stack by the thread module
        0,          // Use minimal stack size
        THREAD_PRIORITY_MAIN - 1, // Priority, just below the main thread
        THREAD_CREATE_STACKTEST,  // Option to test stack size
        led_blinker,
        NULL,
        "led_blinker"
    );

    // Main thread does not need to do anything else
    while (1) {
        thread_yield(); // Yield to other threads
    }

    return 0;
}