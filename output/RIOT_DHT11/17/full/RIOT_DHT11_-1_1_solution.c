#include <xtimer.h>
#include <stdio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 2) // GPIO2 on ESP32
#define SLEEP_INTERVAL (5U * US_PER_SEC) // Sleep interval of 5 seconds

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    int rc;

    rc = dht_init(&dev, &(dht_params_t){ .pin = DHT_PIN, .type = DHT11 });
    while (rc != 0) {
        printf("DHT sensor initialization failed. Retrying...\n");
        xtimer_sleep(1);
        rc = dht_init(&dev, &(dht_params_t){ .pin = DHT_PIN, .type = DHT11 });
    }
    printf("DHT sensor initialized successfully.\n");

    while (1) {
        rc = dht_read(&dev, &temperature, &humidity);
        if (rc == 0) {
            printf("Temperature: %d.%d°C Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT sensor.\n");
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}