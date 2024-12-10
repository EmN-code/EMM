
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void read_dht(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_PIN, DHT_TYPE_DHT11, DHT_PARAM_PULL) < 0) {
        puts("DHT11 initialization failed");
        return;
    }

    if (dht_read(&dev, &temp, &hum) < 0) {
        puts("DHT11 read failed");
        return;
    }

    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
           (int)temp / 10, (int)temp % 10, (int)hum / 10, (int)hum % 10);
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        read_dht();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    return 0;
}
