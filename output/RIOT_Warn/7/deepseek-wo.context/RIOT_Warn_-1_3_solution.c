
#include "periph/gpio.h"
#include "periph/dht.h"
#include "xtimer.h"
#include <stdio.h>

#define DHT_PIN             GPIO_PIN(0, 12)  // GPIO12 on ESP32
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO2 on ESP32
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define INTERVAL            5                // Interval in seconds

int main(void)
{
    dht_t dev;
    int16_t temp, hum;
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize DHT11 sensor
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        puts("DHT11 initialization failed");
        return 1;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            // Check if temperature exceeds the threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on the LED
            } else {
                gpio_clear(LED_PIN);  // Turn off the LED
            }
        } else {
            puts("Failed to read from DHT11 sensor");
        }

        // Wait for the next reading
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
