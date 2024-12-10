
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_TYPE            DHT11
#define SAMPLE_INTERVAL     (5 * 1000 * 1000U)  /* 5 seconds in microseconds */

static dht_t dht_dev;

void read_dht(void)
{
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read from DHT sensor");
    }
}

int main(void)
{
    /* Initialize the DHT11 sensor */
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE,
        .in_mode = GPIO_IN_PU  /* Use GPIO_IN_PU for pull-up */
    };

    if (dht_init(&dht_dev, &dht_params) != DHT_OK) {
        puts("DHT11 initialization failed");
        return 1;
    }

    /* Set up the timer for periodic reading */
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        read_dht();
        xtimer_periodic_wakeup(&last_wakeup, SAMPLE_INTERVAL);
    }

    return 0;
}
