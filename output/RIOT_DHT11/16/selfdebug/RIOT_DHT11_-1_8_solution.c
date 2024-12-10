#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT11_PIN GPIO_PIN(0, 2)  // GPIO 2 is often labeled as 0 in boards like ESP32
#define SLEEP_INTERVAL (5U * US_PER_SEC)  // 5 seconds

int main(void) {
    // Initialize the DHT11 sensor
    dht_params_t dht_params;
    dht_params.pin = DHT11_PIN;
    dht_params.type = DHT11;
    dht_t dev;

    if (dht_init(&dev, &dht_params) != DHT_OK) {
        puts("DHT11 initialization failed!");
        return 1;
    }

    while (1) {
        int16_t temperature, humidity;

        // Read values from DHT11 sensor
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            puts("Failed to read from DHT11 sensor");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, abs(temperature % 10),
                   humidity / 10, humidity % 10);
        }

        // Sleep for 5 seconds
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }

    return 0;
}