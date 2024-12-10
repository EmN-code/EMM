#include <stdlib.h>  // Used for general memory management functions
#include <stdio.h>   // Used for printf debugging and input/output operations
#include <string.h>  // Used for string manipulation functions
#include "periph/gpio.h"  // Used for working with GPIO pins
#include "xtimer.h"       // Used for the timer functionalities
#include "dht.h"          // Used for interacting with the DHT sensor

#define DHT_PIN GPIO_PIN(PORT_A, 7)  // Set the DHT pin
#define LED_PIN GPIO_PIN(PORT_B, 5)  // Set the LED pin
#define TEMP_THRESHOLD 30            // Define temperature threshold

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    
    // Initialize DHT11 sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != 0) {
        printf("Error: Unable to initialize DHT11 sensor\n");
        return 1;
    }

    // Initialize LED as output
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // Read values from the DHT11 sensor
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Error: Unable to read data from DHT11 sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, abs(temp % 10), hum / 10, hum % 10);
            // Check if temperature exceeds threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}