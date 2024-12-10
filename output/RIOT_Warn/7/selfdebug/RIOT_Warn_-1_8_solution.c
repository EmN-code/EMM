#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_A, 1)
#define DHT_TYPE DHT11
#define TEMP_THRESHOLD 30  // Temperature threshold for LED warning

int main(void) {
    int16_t temp, hum;
    dht_t dev;
    gpio_init(LED_PIN, GPIO_OUT);
    int init_res = dht_init(&dev, DHT_PIN, DHT_TYPE);
    if (init_res != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        if (dht_measure(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 >= TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: Temperature above threshold!\n");
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_sleep(5);
    }

    return 0;
}