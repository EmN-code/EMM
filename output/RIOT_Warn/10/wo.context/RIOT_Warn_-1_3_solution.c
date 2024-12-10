#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)  // Choose appropriate port

#define THRESHOLD_TEMPERATURE 30  // Temperature threshold to trigger LED
#define LED_PIN GPIO_PIN(PORT_B, 0) // Choose appropriate port

dht_t dev;

void check_temperature_and_warn(void) {
    int16_t temperature;
    int16_t humidity;
    if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
        printf("Temperature: %d.%d°C
", temperature / 10, temperature % 10);
        printf("Humidity: %d.%d%%
", humidity / 10, humidity % 10);

        if (temperature / 10 > THRESHOLD_TEMPERATURE) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        puts("Failed to read from DHT sensor");
    }
}

int main(void) {
    // GPIO Initialization
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize DHT sensor
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    // Repeat temperature check every 5 seconds
    while (1) {
        check_temperature_and_warn();
        xtimer_sleep(5);
    }

    return 0;
}