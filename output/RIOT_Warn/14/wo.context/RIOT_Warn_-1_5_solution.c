#include <xtimer.h>
#include <stdio.h>
#include "dht.h"
#include "dht_params.h"
#include <periph/gpio.h>

#define DHT_TYPE DHT11
#define DHT_PIN GPIO_PIN(PORT_A, 5)
#define TEMP_THRESHOLD 30 // Example threshold of 30 degrees Celsius

void *read_dht(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT_TYPE;

    dht_init(&dev, &params);

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(GPIO_PIN(PORT_A, 6)); // Turn on LED
            } else {
                gpio_clear(GPIO_PIN(PORT_A, 6)); // Turn off LED
            }
        } else {
            printf("Failed to read from DHT11\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    gpio_init(GPIO_PIN(PORT_A, 6), GPIO_OUT);
    read_dht(NULL);
    return 0;
}