#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2) // Modify this according to your ESP32's LED pin
#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second interval

void toggle_led(void) {
    int rc;
    // Check the current value and toggle the LED pin
    rc = gpio_read(LED_PIN);
    if (rc < 0) {
        printf("Error reading LED pin\n");
        return;
    }
    rc = gpio_write(LED_PIN, !rc);
    if (rc != 0) {
        printf("Error toggling LED\n");
    }
}

int main(void) {
    // Initialize the GPIO for the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing LED pin\n");
        return 1;
    }

    // Initialize the timer and enter a loop
    while (1) {
        toggle_led();
        xtimer_sleep(TIMER_INTERVAL);
    }

    return 0;
}