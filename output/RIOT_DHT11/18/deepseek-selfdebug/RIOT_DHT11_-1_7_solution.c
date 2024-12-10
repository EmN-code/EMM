
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN         GPIO_PIN(0, 2)  // GPIO 2 on ESP32
#define SLEEP_INTERVAL  (5 * 1000000U)  // 5 seconds

int main(void)
{
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT11 sensor
    if (dht_init(&dev, (gpio_t)DHT_PIN) == DHT_OK) {
        printf("DHT11 sensor initialized successfully\n");
    } else {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }

        // Sleep for 5 seconds
        xtimer_sleep(SLEEP_INTERVAL / 1000000U);
    }

    return 0;
}
