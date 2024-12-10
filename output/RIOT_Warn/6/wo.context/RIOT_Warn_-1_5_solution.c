#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 6) // Adjust this to your pin connected to DHT11
#define LED_PIN GPIO_PIN(PORT_B, 0) // Adjust this to your pin connected to the LED
#define THRESHOLD_TEMP 30 // Temperature threshold for the LED warning
#define INTERVAL 5 * US_PER_SEC // 5 seconds interval

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    dht_params_t params;

    params.pin = DHT_PIN;
    params.type = DHT11;
    dht_init(&dev, &params);

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
            if (temperature / 10 > THRESHOLD_TEMP) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read DHT11 sensor\n");
        }
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }
    return 0;
}