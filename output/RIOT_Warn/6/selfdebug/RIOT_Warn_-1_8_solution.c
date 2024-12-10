#include <xtimer.h>  // Timer functions
#include <stdio.h>   // Standard I/O
#include "dht.h"    // DHT sensor
#include "periph/gpio.h" // GPIO functions

#define DHT_PIN GPIO_PIN(PORT_X, X)  // Define the DHT sensor pin
#define LED_PIN GPIO_PIN(PORT_X, X)  // Define the LED pin
#define TEMP_THRESHOLD 30  // Temperature threshold in Celsius

static dht_t dev;

int main(void) {
    // Initialize GPIO pins
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    // Initialize the DHT sensor
    if(dht_init(&dev, DHT_PIN) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            if (temp / 10 > TEMP_THRESHOLD) {
                // Turn on LED warning
                gpio_set(LED_PIN);
            } else {
                // Turn off LED
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5); // Sleep for 5 seconds
    }
    return 0;
}