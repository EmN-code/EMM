#include <xtimer.h>    // Timer related functions
#include <periph/gpio.h> // GPIO related functions
#include "led.h"         // For controlling ESP32 LEDs

#define INTERVAL (1U * US_PER_SEC)
#define LED_PIN GPIO_PIN(PORT_A, 0)  // Assuming LED is connected here

void toggle_led(void)
{
    static int state = 0;
    if (state == 0) {
        gpio_set(LED_PIN);
        state = 1;
    } else {
        gpio_clear(LED_PIN);
        state = 0;
    }
}

int main(void)
{
    // Ensure the GPIO pin is configured for output
    gpio_init(LED_PIN, GPIO_OUT);

    // Main loop; with exception handling and retry logic for external system interaction
    while (1) {
        toggle_led();

        if (xtimer_sleep(INTERVAL / US_PER_SEC) != 0) {
            printf("xtimer_sleep failed, retrying...\n");
            continue; // Retry
        }

        printf("Toggled LED\n");
    }
    return 0;
}