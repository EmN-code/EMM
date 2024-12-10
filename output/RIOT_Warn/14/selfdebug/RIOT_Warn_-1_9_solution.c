#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <dht.h>
#include <xtimer.h>
#include <periph/gpio.h>

// Define the GPIO pins for DHT11 and LED
#define DHT11_PIN GPIO_PIN(PORT_B, 5)
#define LED_PIN GPIO_PIN(PORT_A, 7)

// Define the temperature threshold
#define TEMP_THRESHOLD 30 // Temperature threshold in degree Celsius

int main(void) {
    // Initialize the GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);
    
    // Initialize the DHT sensor
    dht_t dev;
    if (dht_init(&dev, DHT11_PIN, DHT_TYPE_DHT11) != 0) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    while (1) {
        int16_t temperature, humidity;
        if (dht_measure(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Failed to read from DHT11 sensor\n");
        } else {
            printf("Temperature: %d.%d°C\n", temperature / 10, temperature % 10);
            printf("Humidity: %d.%d%%\n", humidity / 10, humidity % 10);

            // Check if temperature exceeds threshold
            if (temperature / 10 > TEMP_THRESHOLD) {
                printf("Temperature exceeds threshold! Turning on LED\n");
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}