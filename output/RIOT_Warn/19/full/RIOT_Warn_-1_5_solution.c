#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN         GPIO_PIN(0, 4)   // Define the pin connected to DHT11 data pin
#define LED_PIN         GPIO_PIN(0, 5)   // Define the pin connected to LED
#define TEMP_THRESHOLD  30               // Temperature threshold in degrees Celsius
#define INTERVAL        5                // Interval to read the DHT11 sensor in seconds

int main(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);
  
    // Initialize DHT sensor
    dht_t dev;
    int16_t temperature, humidity;

    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor\n");
        return -1;
    }

    while (1) {
        // Read data from DHT11 sensor
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            // Print temperature and humidity
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);

            // If temperature exceeds threshold, turn on LED
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }

        // Wait for the interval duration
        xtimer_sleep(INTERVAL);
    }
    return 0;
}