#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT11_PIN GPIO_PIN(0, 2)   // GPIO2 for ESP32
#define DHT_TYPE DHT11
#define DELAY 5

static dht_t dev;

int main(void) {
    // Initialize DHT sensor
    if (dht_init(&dev, DHT11_PIN, DHT_TYPE) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }

        // Sleep for 5 seconds
        xtimer_sleep(DELAY);
    }
    return 0;
}