#include <xtimer.h>  // Timer header for using xtimer functions
#include <stdio.h>   // Standard input-output library
#include "dht.h"    // DHT sensor library
#include "periph/gpio.h" // GPIO library for controlling the LED

#define SENSOR_PIN GPIO_PIN(PORT_B, 5) // Assume DHT11 is connected to Port B Pin 5
#define LED_PIN GPIO_PIN(PORT_A, 0)    // Assume LED is connected to Port A Pin 0
#define TEMP_THRESHOLD 30              // Temperature threshold in Celsius
#define INTERVAL (5U * US_PER_SEC)     // 5 seconds interval

// Main function
int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    
    // Initialize the DHT sensor
    if (dht_init(&dev, SENSOR_PIN) != DHT_OK) {
        puts("Error: Failed to initialize DHT sensor");
        return 1;
    }

    // Set LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Infinite loop to read sensor data and control LED
    while (1) {
        // Read data from the DHT sensor
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature / 10, humidity / 10);
            
            // Turn on LED if temperature exceeds threshold
            if ((temperature / 10) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            }
            else {
                gpio_clear(LED_PIN);
            }
        }
        else {
            puts("Error: Failed to read from DHT sensor");
        }

        // Wait for the defined interval
        xtimer_sleep(5);
    }

    return 0;
}