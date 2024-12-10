
#include "periph/gpio.h"
#include "dht.h"
#include "xtimer.h"
#include "log.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static dht_t dht_dev;

int main(void)
{
    int16_t temp, hum;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Initialize the DHT11 sensor
    if (dht_init(&dht_dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        LOG_ERROR("DHT11 initialization failed\n");
        return 1;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dht_dev, &temp, &hum) != DHT_OK) {
            LOG_ERROR("Failed to read data from DHT11\n");
        } else {
            LOG_INFO("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                     temp / 10, temp % 10, hum / 10, hum % 10);
        }

        // Sleep for 5 seconds
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    return 0;
}
