#include <thread.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <xtimer.h>

#define LED_PIN GPIO_PIN(0, 2)  // Replace with your actual pin

void *led_blinker(void *arg) {
    (void)arg; // Avoid unused parameter warning

    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        gpio_toggle(LED_PIN);
        xtimer_sleep(1); // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    puts("Starting LED Blink Timer on ESP32");

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, led_blinker, NULL, "blinker");

    return 0;
}