#include <xtimer.h>  // including xtimer for delay functions
#include <stdio.h>
#include <periph/gpio.h> // including gpio for controlling LED
#include "dht.h" // including dht library for temperature sensor

#define DHT_PIN GPIO_PIN(0, 4) // DHT11 is connected to GPIO 4
#define LED_PIN GPIO_PIN(0, 2) // LED is connected to GPIO 2
#define TEMPERATURE_THRESHOLD 30 // temperature threshold to trigger LED
#define DELAY 5 // Delay in seconds

int main(void) {
    // Initialize DHT sensor
    dht_t dev;
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        int16_t temperature, humidity;

        // Read the sensor
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
            
            // Check if temperature exceeds threshold
            if (temperature / 10 > TEMPERATURE_THRESHOLD) {
                printf("Temperature exceeds threshold! Turning on LED.\n");
                gpio_write(LED_PIN, 1);
            } else {
                gpio_write(LED_PIN, 0);
            }
        }

        // Wait for the next measurement
        xtimer_sleep(DELAY);
    }

    return 0;
}