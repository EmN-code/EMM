#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 4)  // Assume the DHT11 sensor is connected to GPIO pin 4
#define LED_PIN GPIO_PIN(0, 5)  // Assume the LED is connected to GPIO pin 5
#define TEMP_THRESHOLD 30  // Temperature threshold in Celsius

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_t dev;
    int16_t temperature, humidity;

    if (dht_init(&dev, DHT11, GPIO_PIN(0, 4)) != DHT_OK) {
        puts("Error initializing DHT11 sensor");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            puts("Error reading data from DHT11 sensor");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        xtimer_sleep(5);
    }

    return 0;
}