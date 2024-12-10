#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 4)
#define LED_PIN GPIO_PIN(0, 2)
#define TEMP_THRESHOLD 30

int main(void) {
    dht_t dev;
    int16_t temperature;
    int16_t humidity;

    if (dht_init(&dev, DHT_TYPE_DHT11) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        if (dht_read(&dev, &temperature, &humidity, DHT_PIN) == DHT_OK) {
            printf("Temperature: %d°.%dC, Humidity: %d.%d%%\n", 
                   temperature / 10, abs(temperature % 10), 
                   humidity / 10, abs(humidity % 10));

            if (temperature > (TEMP_THRESHOLD * 10)) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read data from DHT sensor\n");
        }

        xtimer_sleep(5);
    }
    return 0;
}