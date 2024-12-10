#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define READ_INTERVAL (5U * US_PER_SEC)

dht_t dht_dev;

int main(void)
{
    int16_t temperature, humidity;

    if (dht_init(&dht_dev, DHT_TYPE_DHT11, DHT_PIN) != 0) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT11\n");
        }

        xtimer_sleep(5);
    }

    return 0;
}