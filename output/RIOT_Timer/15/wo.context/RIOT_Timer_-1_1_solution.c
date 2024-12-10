#include "xtimer.h"
#include "gpio.h"
#include <stdio.h>

#define LED_PIN    GPIO_PIN(0, 2)  // Example pin, change as necessary

void toggle_led(void) {
    static int led_status = 0;
    led_status = !led_status;
    gpio_write(LED_PIN, led_status);
    printf("LED is now %s\n", led_status ? "ON" : "OFF");
}

int main(void) {
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        printf("Failed to initialize LED pin\n");
        return 1;
    }

    printf("Starting timer every second to toggle LED\n");

    while(1) {
        xtimer_sleep(1);
        toggle_led();
    }

    return 0;  // Although this point is never reached
}