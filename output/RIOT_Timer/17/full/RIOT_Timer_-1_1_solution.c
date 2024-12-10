#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"

#define INTERVAL (1U * US_PER_SEC) // 1 second interval
#define LED_PIN GPIO_PIN(PORT_A, 10)  // Assuming LED is connected to PA10, change as needed

void *led_blinker(void *arg) {
    (void)arg;
    while (1) {
        // Toggle LED state
        gpio_toggle(LED_PIN);
        // Sleep for the given interval
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }
    return NULL;
}

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Declare a thread stack for LED blinker
    char led_blinker_stack[THREAD_STACKSIZE_MAIN];

    // Create the thread for LED blinking
    thread_create(led_blinker_stack, sizeof(led_blinker_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, led_blinker, NULL, "led_blinker");
    
    // Main should not return
    while (1) {
        thread_sleep(); // keep the main thread sleeping
    }
    return 0;
}