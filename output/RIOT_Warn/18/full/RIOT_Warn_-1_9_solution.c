#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_A, 6) // Define the GPIO pin where the DHT11 is connected
#define LED_PIN GPIO_PIN(PORT_B, 7) // Define the GPIO pin where the LED is connected
#define TEMP_THRESHOLD 30           // Define the temperature threshold
#define DHT_TYPE DHT11

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    // Initialize the DHT sensor
    dht_t dev;
    if (dht_init(&dev, DHT_PIN, DHT_TYPE) != 0) {
        printf("Error initializing DHT sensor\n");
        return 1;
    }

    int16_t temp, hum;

    while (1) {
        // Read temperature and humidity from the DHT sensor
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Error reading DHT sensor\n");
            xtimer_sleep(5); // Retry after 5 seconds
            continue;
        }

        // Convert temperature to Celsius
        int temperature = temp / 10;
        printf("Temperature: %d°C, Humidity: %d%%\n", temperature, hum / 10);

        // Check if temperature exceeds the threshold
        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN); // Turn on LED
            printf("Temperature threshold exceeded! LED turned on.\n");
        } else {
            gpio_clear(LED_PIN); // Turn off LED
        }

        xtimer_sleep(5); // Wait for 5 seconds before reading again
    }
    return 0;
}