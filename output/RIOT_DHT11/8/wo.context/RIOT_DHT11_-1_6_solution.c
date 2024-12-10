#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2) // GPIO2 pin definition
#define DHT_TYPE DHT11 // The type of DHT sensor

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Initialize the GPIO pin for the DHT sensor
    if (dht_init(&dev, DHT_TYPE, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        // Read values from the DHT sensor
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            // Convert temperature to Celsius
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                    temperature / 10, temperature % 10,
                    humidity / 10, humidity % 10);
        }
        // Wait for 5 seconds
        xtimer_periodic_wakeup(&last_wakeup, 5 * US_PER_SEC);
    }

    return 0;
}