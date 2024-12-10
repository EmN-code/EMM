
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;

    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != 0) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            puts("Failed to read data from DHT11 sensor");
        }

        xtimer_sleep(5);
    }

    return 0;
}
