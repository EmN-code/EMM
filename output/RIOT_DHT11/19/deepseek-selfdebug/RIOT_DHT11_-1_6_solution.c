
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define SLEEP_INTERVAL      (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        puts("DHT11 initialization failed");
        return 1;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            puts("Failed to read data from DHT11");
        }

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}