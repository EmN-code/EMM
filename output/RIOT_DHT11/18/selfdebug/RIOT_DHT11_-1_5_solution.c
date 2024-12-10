#include <xtimer.h> 
#include <stdio.h>
#include <periph/gpio.h>
#include <string.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 2) // GPIO pin for DHT (GPIO 2)
#define DHT_TYPE DHT11
#define DELAY 5U * 1000000U // 5 seconds in microseconds

static dht_t dht_dev;

int main(void) {
    int res;
    int16_t temp, hum;

    // Initialize DHT sensor
    if (dht_init(&dht_dev, DHT_PIN, DHT_TYPE) != 0) {
        printf("DHT initialization failed\n");
        return 1;
    }

    printf("Successfully initialized DHT sensor\n");

    while (1) {
        // Read temperature and humidity
        res = dht_read(&dht_dev, &temp, &hum);
        if (res == DHT_OK) {
            printf("Temperature: %d.%d°C\n", temp / 10, temp % 10);
            printf("Humidity: %d.%d%%\n", hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Wait for the specified delay
        xtimer_sleep(DELAY);
    }

    return 0;
}