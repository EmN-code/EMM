#include <xtimer.h>  // For timing
#include <stdio.h>  // For printf function
#include "dht.h"  // For DHT11 sensor-related functions
#include <periph/gpio.h> // For GPIO control

#define DHT_PIN GPIO_PIN(PORT_A, 0)  // Define the DHT data pin
#define LED_PIN GPIO_PIN(PORT_B, 0)  // Define the LED pin
#define TEMP_THRESHOLD 30  // Threshold temperature in °C

int main(void)
{
    // Initialize GPIO
    gpio_init(LED_PIN, GPIO_OUT);
    dht_t dev;
    // Initialize DHT11
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }
    int16_t temp, humidity;
    while (1) {
        if (dht_read(&dev, &temp, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, humidity / 10, humidity % 10);

            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read DHT11 sensor\n");
        }
        xtimer_sleep(5);  // Wait for 5 seconds before next reading
    }
    return 0;
}