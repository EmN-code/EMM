#include <xtimer.h>  // For time sleeping
#include <stdio.h>
#include <dht.h>  // DHT sensor API
#include <board.h>  // Board configuration
#include <periph/gpio.h>  // GPIO peripheral for LED manipulation

// Threshold temperature value
#define TEMP_THRESHOLD 30

// GPIO pin for DHT11
#define DHT_PIN GPIO_PIN(PORT_A, 0)

// GPIO pin for LED
#define LED_PIN GPIO_PIN(PORT_B, 1)

// DHT device object
static dht_t dht;

int main(void) {
    // Initialize DHT
    dht_params_t params;
    dht_init(&dht, &params);

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // Variables to hold sensor results
        int16_t temperature, humidity;

        // Read data from DHT sensor
        int res = dht_read(&dht, &temperature, &humidity);
        if(res != DHT_OK) {
            printf("Failed to read DHT11 sensor\n");
        } else {
            // Print temperature and humidity
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

            // Check temperature threshold
            if (temperature > TEMP_THRESHOLD) {
                // Turn on the LED
                gpio_set(LED_PIN);
            } else {
                // Turn off the LED
                gpio_clear(LED_PIN);
            }
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}