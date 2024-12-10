#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT11_PIN GPIO_PIN(PORT_A, 0) // Define the GPIO pin connected to DHT11
#define LED_PIN GPIO_PIN(PORT_A, 1)   // Define the GPIO pin connected to the LED
#define TEMP_THRESHOLD 30            // Temperature threshold for warning in Celsius

static dht_t dev;

int main(void) {
    // Initialize GPIOs
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);  // Turn off the LED
    dht_init(&dev, DHT11_PIN, DHT11);

    while (1) {
        // Read DHT11 sensor
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp/10, temp%10, hum/10, hum%10);
            
            // Check temperature threshold and turn on LED if exceeded
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on the LED
                printf("Warning: Temperature exceeds threshold!\n");
            } else {
                gpio_clear(LED_PIN);  // Turn off the LED
            }
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
        
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}