#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>  // Assuming the DHT library provides dht_t and necessary functions

// Define GPIO pin
#define DHT_PIN GPIO_PIN(0, 2)

// Define the DHT type
#define DHT_TYPE DHT11

// Timer interval for 5 seconds
#define INTERVAL (5U * 1000000U)

int main(void) {
    // Storage for the DHT device
    dht_t dev;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT_TYPE) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor.\n");
        return -1;
    }
    
    int16_t temp, hum;
    while (1) {
        // Read values from the DHT sensor
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %2.1f°C, Humidity: %2.1f%%\n", temp / 10.0, hum / 10.0);
        } else {
            printf("Error reading DHT11 sensor.\n");
        }
        
        // Sleep for the defined interval
        xtimer_sleep(INTERVAL / 1000000U);
    }
    
    return 0;
}