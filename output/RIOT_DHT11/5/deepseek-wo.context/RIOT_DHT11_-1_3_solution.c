
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5 * 1000 * 1000U) /* 5 seconds in microseconds */

static dht_t dev;

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();

    /* Initialize the DHT11 sensor */
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        puts("DHT11 initialization failed");
        return 1;
    }

    while (1) {
        int16_t temp, hum;

        /* Read temperature and humidity */
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("DHT11 read failed");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }

        /* Sleep for 5 seconds */
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    return 0;
}
