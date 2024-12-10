#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_B, 5)  // Assuming the LED is connected to port B, pin 5
#define INTERVAL (1U * US_PER_SEC)   // One second interval

void setup_timer_and_led(void) {
    // Initialize the LED pin as output
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error: Unable to initialize GPIO\n");
        return;
    }

    while (1) {
        // Turn on the LED
        gpio_set(LED_PIN);
        printf("LED is ON\n");

        // Sleep for the specified interval
        xtimer_sleep(1);

        // Turn off the LED
        gpio_clear(LED_PIN);
        printf("LED is OFF\n");

        // Sleep for the specified interval
        xtimer_sleep(1);
    }
}

int main(void) {
    printf("Timer and LED setup starting\n");
    setup_timer_and_led();
    return 0;
}