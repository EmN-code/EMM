
#include <stdio.h>
#include "xtimer.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2 on ESP32
#define INTERVAL (5 * 1000 * 1000U)  // 5 seconds in microseconds

int main(void)
{
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT11 sensor on the specified GPIO pin
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) == DHT_OK) {
        printf("DHT11 sensor initialized successfully\n");
    } else {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    while (1) {
        // Read temperature and humidity from the DHT11 sensor
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   (int)temp / 10, (int)temp % 10, (int)hum / 10, (int)hum % 10);
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }

        // Wait for 5 seconds before the next reading
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
