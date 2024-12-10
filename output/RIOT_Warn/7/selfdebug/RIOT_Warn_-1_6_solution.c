#include <xtimer.h>  // For timer utilities
#include <timex.h>   // For timex time conversion utilities
#include <stdio.h>   // For standard input/output
#include <periph/gpio.h>  // For GPIO functions
#include "dht.h"    // For DHT sensor functions

#define DHT_PIN GPIO_PIN(PORT_A, 1)  // Define the DHT sensor pin
#define LED_PIN GPIO_PIN(PORT_B, 2)  // Define the LED pin
#define TEMP_THRESHOLD 25  // Temperature threshold value
#define DELAY 5  // Delay in seconds

static dht_t dev;

int main(void) {
    if (dht_init(&dev, DHT_TYPE_DHT11) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }

    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d, Humidity: %d\n", temp / 10, hum / 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                // Temperature exceeds threshold, turn on LED
                gpio_set(LED_PIN);
            } else {
                // Temperature is below threshold, turn off LED
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read data from DHT sensor\n");
        }
        xtimer_sleep(DELAY);
    }
    return 0;
}
