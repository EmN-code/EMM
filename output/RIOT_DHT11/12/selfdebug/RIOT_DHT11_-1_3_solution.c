#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    dht_t dev;

    // Initialize the DHT device without specifying type
    if (dht_init(&dev, DHT_PIN) != 0) {
        printf("Error initializing DHT sensor\n");
        return -1;
    }

    while (1) {
        int16_t temp, hum;

        // Read temperature and humidity from DHT11 sensor
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Error reading DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }

        // Sleep for the specified interval
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}