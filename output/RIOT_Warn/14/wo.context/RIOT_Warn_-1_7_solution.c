#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 10)
#define THRESHOLD_TEMPERATURE 30
#define LED_PIN GPIO_PIN(PORT_B, 5)

int main(void) {
    int16_t temperature, humidity;
    dht_t dev;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_TYPE_DHT11) == DHT_OK) {
        printf("DHT11 sensor initialized successfully.\n");
    } else {
        printf("Error initializing DHT11 sensor.\n");
        return 1;
    }

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
            if (temperature > THRESHOLD_TEMPERATURE) {
                gpio_set(LED_PIN);
                printf("Warning: Temperature exceeded the threshold!\n");
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor!\n");
        }
        xtimer_sleep(5);
    }
    return 0;
}