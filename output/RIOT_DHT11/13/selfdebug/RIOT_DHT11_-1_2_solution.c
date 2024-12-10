#include "periph/gpio.h"
#include "kernel.h"
#include "xtimer.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2
#define READ_INTERVAL (5U * US_PER_SEC)  // 5 seconds in microseconds

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    while (1) {
        // Read the temperature and humidity values
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            puts("Failed to read from DHT11 sensor");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        }

        // Delay for the specified read interval
        xtimer_sleep(READ_INTERVAL / US_PER_SEC);
    }

    return 0;
}