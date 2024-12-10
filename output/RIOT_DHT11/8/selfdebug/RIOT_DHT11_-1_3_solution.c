#include <xtimer.h> // Include xtimer.h for time functions
#include <stdio.h> // Include stdio.h for input/output
#include "dht.h"  // Include dht.h for sensor operation
#include "dht_params.h"  // Include dht_params.h for sensor parameters

// Create a DHT device descriptor
dht_t dev;

int main(void) {
    // Initialize the DHT sensor on the specified GPIO pin
    if (dht_init(&dev, GPIO2, DHT11) != DHT_OK) {
        // Print an error message if initialization fails
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    // Infinite loop to continually read sensor values
    while(1) {
        int16_t temp, hum;
        // Read temperature and humidity data
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            // Output the temperature and humidity readings
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            // Print an error message if a read fails
            printf("Failed to read data from DHT11 sensor\n");
        }
        
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    
    return 0; // End of main function
}