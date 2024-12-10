#include <xtimer.h>  // For managing timers and delaying execution
#include <stdio.h>    // For printf function
#include <dht.h>      // For interfacing with DHT sensor
#include <gpio.h>     // For controlling GPIO pins for the LED

// Define DHT parameters
static dht_t dht;
static const dht_params_t dht_params = {
    .pin = GPIO_PIN(0, 2),  // Pin where the DHT11 data pin is connected
    .type = DHT11           // Specify the sensor type
};

// LED warning threshold (in degrees Celsius)
#define TEMP_THRESHOLD 30

// LED GPIO pin
#define LED_PIN GPIO_PIN(0, 3)

int main(void) {
    // Initialize DHT sensor
    if (dht_init(&dht, &dht_params) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    int16_t temp, hum;

    while (1) {
        // Read temperature and humidity from DHT sensor
        if (dht_read(&dht, &temp, &hum) != 0) {
            printf("Failed to read DHT sensor\n");
        } else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);
            
            // Check if temperature exceeds threshold
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
                printf("Warning: High temperature detected!\n");
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}