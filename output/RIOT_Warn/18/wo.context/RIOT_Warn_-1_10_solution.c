#include "periph/gpio.h"
#include "dht.h"
#include "xtimer.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_A, 1)
#define TEMP_THRESHOLD 30  // temperature in Celsius
#define DHT_TYPE DHT11

static dht_t dht_dev;

void *read_dht(void *arg) {
    (void)arg;
    int16_t temperature, humidity;

    while(1) {
        if(dht_read(&dht_dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C\n", temperature / 10, temperature % 10);
            printf("Humidity: %d.%d%%\n", humidity / 10, humidity % 10);

            if(temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }

        } else {
            printf("Failed to read DHT sensor\n");
        }

        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    if (dht_init(&dht_dev, DHT_PIN, DHT_TYPE) == DHT_OK) {
        puts("DHT sensor initialized");
    } else {
        puts("Error initializing DHT sensor");
    }

    read_dht(NULL);

    return 0;
}