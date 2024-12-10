
#include "periph/gpio.h"
#include "periph/timer.h"
#include "dht.h"
#include "xtimer.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define TIMER_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static void read_dht(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        puts("DHT11 initialization failed");
        return;
    }

    if (dht_read(&dev, &temp, &hum) != DHT_OK) {
        puts("DHT11 read failed");
        return;
    }

    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
           temp / 10, temp % 10, hum / 10, hum % 10);
}

int main(void) {
    xtimer_t timer;

    xtimer_init(&timer);
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        read_dht();
        xtimer_usleep(TIMER_INTERVAL);
    }

    return 0;
}
