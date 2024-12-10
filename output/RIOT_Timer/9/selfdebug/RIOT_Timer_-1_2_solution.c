#include <stdio.h>
#include <periph/gpio.h>
#include <thread.h>
#include <xtimer.h>

#define INTERVAL (1U * US_PER_SEC) // 1 second interval
#define LED_PIN GPIO_PIN(0, 2) // Define the pin where the LED is connected

void *led_blink(void *arg) {
    (void)arg; // Avoid unused parameter warning
    while (1) {
        gpio_toggle(LED_PIN);
        xtimer_sleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    // Initialize GPIO pin for output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a thread for LED blinking
    thread_create((char *)led_blink, NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, "led_blink");

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}