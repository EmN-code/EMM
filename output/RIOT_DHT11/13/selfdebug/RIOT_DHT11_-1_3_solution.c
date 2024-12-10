#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <gpio.h>
#include <kernel.h>

#define DHT11_PIN (GPIO_PIN(PORT_A, 2))  // Use GPIO 2 for DHT11
#define INTERVAL (5U * US_PER_SEC)       // 5 seconds interval

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT11_PIN) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor.\n");
        return 1;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%dC, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11 sensor.\n");
        }

        // Sleep for the specified interval
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}