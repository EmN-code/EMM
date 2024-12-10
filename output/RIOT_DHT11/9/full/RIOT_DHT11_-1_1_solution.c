#include <xtimer.h>
#include <dht.h>
#include <stdio.h>

#define DHT_PIN (GPIO_PIN(0, 2))
#define DHT_TYPE DHT11
#define SLEEP_TIME 5000000 //5 seconds in microseconds

dht_t dev;

int main(void) {
    printf("Initializing DHT...");
    // Initialize DHT device
    if (dht_init(&dev, &(dht_params_t){.pin = DHT_PIN, .type = DHT_TYPE}) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    int16_t temp, hum;

    while (1) {
        int res = dht_read(&dev, &temp, &hum);
        if (res == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Error reading from DHT sensor\n");
        }

        xtimer_usleep(SLEEP_TIME);
    }

    return 0;
}