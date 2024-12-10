#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 0)
#define INTERVAL (1000 * US_PER_MS) // 1 second interval in microseconds

void toggle_led(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);
    puts("Timer initialized, LED will toggle every second.");

    while (1) {
        toggle_led();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}