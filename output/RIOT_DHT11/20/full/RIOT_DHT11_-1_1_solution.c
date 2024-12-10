#include <xtimer.h>
#include <timex.h>
#include <stdio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 2) // GPIO 2
#define INTERVAL (5U * US_PER_SEC) // 5 seconds interval

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    int res;

    // Initialize the DHT device
    res = dht_init(&dev, DHT_PIN, DHT11);
    if (res != DHT_OK) {
        printf("Error initializing DHT11\n");
        return 1;
    }

    printf("DHT11 sensor initialized on GPIO 2\n");

    while (1) {
        // Read the DHT sensor
        res = dht_read(&dev, &temp, &hum);
        
        // Check if the reading is successful
        if (res == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11 sensor. Retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        }

        // Wait for the next reading interval
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}