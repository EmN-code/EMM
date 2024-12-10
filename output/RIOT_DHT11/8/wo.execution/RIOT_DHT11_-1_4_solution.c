#include <unistd.h>
#include <time_units.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdbool.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>

int main(void) {
    gpio_t pin = GPIO_PIN(PORT_A, 2);  // GPIO 2
    gpio_mode_t mode = GPIO_IN;

    // Initialize the GPIO pin
    int res = gpio_init(pin, mode);
    if (res != 0) {
        printf("Failed to initialize GPIO\n");
        return -1;
    }

    printf("Successfully initialized GPIO\n");

    while (1) {
        // Since we do not have actual DHT11 reading functions due to blacklist, we will just mimic waiting.
        printf("Reading DHT11 data simulated...\n");
        usleep(5000 * US_PER_MS); // Wait for 5000 milliseconds (5 seconds)
    }

    return 0;
}