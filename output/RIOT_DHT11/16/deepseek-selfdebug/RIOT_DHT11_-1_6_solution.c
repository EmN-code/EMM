
#include "periph/gpio.h"
#include "dht.h"
#include "xtimer.h"
#include "log.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_TYPE            DHT11
#define SLEEP_INTERVAL      (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT_PIN, DHT_TYPE) != DHT_OK) {
        LOG_ERROR("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            LOG_INFO("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                     temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            LOG_ERROR("Failed to read data from DHT11 sensor\n");
        }

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}