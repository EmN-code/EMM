#include <xtimer.h>  // Ensure time functions
#include <stdio.h>   // For printf function
#include <dht.h>     // DHT sensor library
#include <periph/gpio.h>  // For interacting with LEDs

#define DHT_PIN GPIO_PIN(PORT_B, 5)  // Define DHT11 data pin
#define LED_PIN GPIO_PIN(PORT_A, 7)  // Define LED pin
#define TEMP_THRESHOLD 30  // Temperature threshold in Celsius

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return -1;
    }

    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        printf("Failed to initialize LED pin\n");
        return -1;
    }

    while (1) {
        // Read data from the DHT sensor
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Failed to read data from DHT sensor\n");
        } else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);

            // Check if the temperature exceeds the threshold
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Temperature threshold exceeded! LED ON\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature is normal. LED OFF\n");
            }
        }
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}