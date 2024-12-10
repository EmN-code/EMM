#include <stdio.h>  // Standard input/output library
#include <stdbool.h> // Boolean data type for C
#include "dht.h"    // DHT sensor library
#include "periph/gpio.h" // Peripheral GPIO library for controlling LED
#include "xtimer.h" // Timer library for RIOT OS

#define DHT_PARAM_PIN GPIO_PIN(PORT_A, 0) // Define the GPIO pin for the DHT11
#define LED_PIN GPIO_PIN(PORT_B, 0)       // Define the GPIO pin for the LED
#define TEMP_THRESHOLD 25                 // Define the temperature threshold
#define INTERVAL (5U * US_PER_SEC)        // Define the interval as 5 seconds (in microseconds)

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PARAM_PIN) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }
    
    while (true) {
        // Read the sensor data
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            
            // Check if temperature exceeds threshold
            if ((temp / 10) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Temperature threshold exceeded! LED is ON.\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature is within the safe range. LED is OFF.\n");
            }
        }
        
        xtimer_sleep(INTERVAL);
    }
    
    return 0;
}