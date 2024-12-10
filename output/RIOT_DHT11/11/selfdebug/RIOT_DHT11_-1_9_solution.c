#include <ztimer.h>
#include <periph/gpio.h>
#include <dht.h>
#include <stdio.h>

#define DHT_TYPE DHT11
#define DHT_PIN  GPIO_PIN(0, 2)
#define INTERVAL (5U * US_PER_SEC)

static ztimer_periodic_t timer;
static dht_t dev;

static void read_and_print_dht(void)
{
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C\n", temp / 10, temp % 10);
        printf("Humidity: %d.%d%%\n", hum / 10, hum % 10);
    }
    else {
        printf("Could not read data from sensor\n");
    }
}

int main(void)
{
    dht_t dev;

    if (dht_init(&dev, DHT_TYPE)) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    ztimer_periodic_init(ZTIMER_MSEC, &timer, read_and_print_dht);
    ztimer_periodic_start(&timer, INTERVAL);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }

    return 0;
}