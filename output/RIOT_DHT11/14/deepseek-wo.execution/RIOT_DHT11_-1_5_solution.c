
#include "periph/gpio.h"
#include "ztimer.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_TYPE            DHT11
#define SLEEP_INTERVAL      (5 * ZTIMER_SEC)

static dht_t dht_dev;

int main(void)
{
    int16_t temperature, humidity;

    /* Initialize the DHT device */
    dht_init(&dht_dev, DHT_TYPE, DHT_PIN);

    while (1) {
        /* Read temperature and humidity */
        if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            puts("Failed to read from DHT sensor");
        }

        /* Sleep for 5 seconds */
        ztimer_sleep(ZTIMER_SEC, SLEEP_INTERVAL);
    }

    return 0;
}
