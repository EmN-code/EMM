#include <xtimer.h>
#include <stdio.h>
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN     GPIO_PIN(0, 2)
#define SLEEP_TIME  (5U * US_PER_SEC)  // 5 seconds

dht_t dev;

int main(void) {
    int16_t temp, hum;
    int result;

    // Initialize the DHT sensor
    while (1) {
        result = dht_init(&dev, DHT_PIN);
        if (result != DHT_OK) {
            printf("Failed to initialize DHT sensor. Retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("DHT sensor initialized successfully.\n");
            break;
        }
    }

    // Main loop: Read every 5 seconds
    while (1) {
        result = dht_read(&dev, &temp, &hum);

        if (result == DHT_OK) {
            printf("Temperature: %d.%dC, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT sensor.\n");
        }

        xtimer_usleep(SLEEP_TIME);
    }

    return 0;
}