#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 4) // Replace PORT_A with appropriate port number for ESP32
#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second interval

static void toggle_led(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Create and initialize the xtimer
    xtimer_t timer;
    timer.callback = toggle_led;
    timer.arg = NULL;

    // Set the timer
    xtimer_set(&timer, TIMER_INTERVAL);

    // Keep the main function running
    while (1) {
        thread_yield();
    }
    return 0;
}