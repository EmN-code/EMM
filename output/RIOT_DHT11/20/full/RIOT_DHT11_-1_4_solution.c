#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(0, 2) // Define the DHT11 data pin

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Initialize the DHT sensor
    while (1) {
        if (dht_init(&dev, DHT_PIN, DHT11) != 0) {
            printf("Failed to initialize DHT sensor\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("DHT sensor initialized\n");
            break;
        }
    }

    while (1) {
        // Read data from the DHT11 sensor
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Failed to read data from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        }

        // Wait for 5 seconds
        xtimer_periodic_wakeup(&last_wakeup, 5 * US_PER_SEC);
    }
    return 0;
}