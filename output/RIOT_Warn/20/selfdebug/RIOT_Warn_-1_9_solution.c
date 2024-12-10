#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 10) // Define the GPIO port and pin connected to the DHT11
#define LED_PIN GPIO_PIN(PORT_B, 2)  // Define the GPIO port and pin connected to the LED
#define TEMP_THRESHOLD 25 // Temperature threshold for LED warning
#define SLEEP_INTERVAL (5U * US_PER_SEC) // 5 seconds interval

int main(void) {
    // Variable declarations
    dht_t dev;
    int16_t temp, hum;

    // Initialize the GPIO for the DHT and LED
    gpio_init(DHT_PIN, GPIO_OUT);
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN); // Turn off LED initially

    // Initialize the DHT sensor with its type and pin
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return -1;
    }

    // Main loop
    while (1) {
        // Read temperature and humidity from the DHT sensor
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp / 10, hum / 10);
            // Check if the temperature exceeds the threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN); // Turn on LED
            } else {
                gpio_clear(LED_PIN); // Turn off LED
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        // Sleep for 5 seconds
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return 0;
}