// This program reads the temperature from a DHT11 sensor every 5 seconds,
// and turns on an LED if the temperature exceeds a specified threshold.

// Import necessary libraries
#include "xtimer.h"           // Provides timer functions for scheduling
#include "periph/gpio.h"     // Provides GPIO functionality
#include "dht.h"             // Provides functions for interacting with DHT sensors

#define LED_PIN GPIO_PIN(PORT_A, 5)  // Define the GPIO pin where the LED is connected
#define DHT_PIN GPIO_PIN(PORT_B, 10) // Define the GPIO pin where the DHT11 sensor is connected
#define TEMP_THRESHOLD 30            // Temperature threshold for warning in degrees Celsius

static dht_params_t dht_params = {
    .pin = DHT_PIN,
    .type = DHT_XXH_SI7021 // Assuming the correct type. Adjust if needed or defined accurately.
};

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);

    // Variable to store temperature
    int16_t temperature;
    int16_t humidity;
    
    while (1) {
        if (dht_read(&dht_params, &temperature, &humidity) == DHT_OK) {
            // Convert temperature to degrees Celsius if needed (depends on the library)
            temperature /= 10;
            // Check if temperature exceeds the threshold
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}