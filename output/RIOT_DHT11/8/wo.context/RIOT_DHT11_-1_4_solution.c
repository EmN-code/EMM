#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT11_TYPE DHT11
#define DHT11_PIN GPIO_PIN(0, 2)
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds

dht_t dev;
uint16_t temperature, humidity;

int main(void) {
    /* initialize the DHT11 sensor */
    if (dht_init(&dev, DHT11_TYPE, DHT11_PIN) != DHT_OK) {
        puts("Error: unable to initialize DHT11");
        return 1;
    }

    while (1) {
        /* read values from DHT11 sensor */
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                    temperature / 10, temperature % 10,
                    humidity / 10, humidity % 10);
        } else {
            puts("Error: unable to read from DHT11 sensor");
        }

        /* sleep for the defined interval */
        xtimer_usleep(INTERVAL);
    }

    return 0;
}