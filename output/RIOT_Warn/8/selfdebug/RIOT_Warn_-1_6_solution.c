// This RIOT code reads data from a DHT11 sensor every 5 seconds and turns on an LED if the
// temperature exceeds a defined threshold. It uses the ESP32 platform.

#include <stdio.h>
#include "xtimer.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT11_PIN GPIO_PIN(0, 4)  // Change this to the actual pin connected to the DHT11
#define LED_PIN GPIO_PIN(0, 2)    // Change this to the actual pin connected to the LED

#define TEMP_THRESHOLD 30.0       // Threshold temperature in Celsius

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    
    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT11_PIN, DHT11) != DHT_OK) {
        puts("Error initializing DHT11 sensor");
        return 1;
    }

    // Initialize the LED GPIO
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        puts("Error initializing LED pin");
        return 1;
    }

    while (1) {
        // Read data from the DHT11 sensor
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            puts("Error reading from DHT11 sensor");
        } else {
            printf("Temperature: %d.%d °C, Humidity: %d.%d %%\n", 
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);

            // Check if the temperature exceeds the threshold
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                puts("Warning: Temperature exceeds threshold!");
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}