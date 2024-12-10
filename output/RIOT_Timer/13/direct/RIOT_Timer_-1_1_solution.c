#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2)  // Define LED pin
#define INTERVAL (1U * US_PER_SEC)  // Set timer interval to 1 second

void *blink(void *arg) {
    (void)arg;
    while (1) {
        gpio_toggle(LED_PIN);  // Toggle the LED state
        xtimer_sleep(1);  // Wait 1 second
    }

    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);  // Initialize the LED pin as output
    
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN, 0, blink, NULL, "blink");  // Create a thread to handle blinking

    // Initial sleep to settle the system (optional)
    xtimer_sleep(1);  

    // Main thread doesn't do anything, other than sleeping forever
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}