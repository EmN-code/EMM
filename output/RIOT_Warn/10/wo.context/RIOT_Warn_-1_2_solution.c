#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0) // Update with the correct port and pin
#define LED_PIN GPIO_PIN(PORT_B, 0) // Update with the correct port and pin
#define TEMP_THRESHOLD 25
#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    while (1) {
        // Read the temperature and humidity
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read data from DHT sensor\n");
        } else {
            printf("Temperature: %d °C, Humidity: %d %%\n", temp, hum);

            // Check if the temperature exceeds the threshold
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Wait for the interval
        xtimer_sleep(INTERVAL);
    }
    return 0;
}