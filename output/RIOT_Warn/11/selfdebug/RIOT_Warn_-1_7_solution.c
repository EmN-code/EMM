#include <xtimer.h> // Include xtimer for timing functions
#include <stdio.h>  // Include for standard IO functions
#include "dht.h"    // Include DHT sensor API
#include "periph/gpio.h"  // Include GPIO peripheral API

#define DHT_PIN GPIO_PIN(PORT_X, 0) // Replace PORT_X with the actual port for your setup
#define LED_PIN GPIO_PIN(PORT_Y, 0) // Replace PORT_Y with the actual port for your setup
#define TEMP_THRESHOLD 30 // Temperature threshold in °C

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_t dev;
    dev.params.pin = DHT_PIN;
    dev.type = DHT11;

    while (1) {
        int16_t temperature, humidity;
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
            // Check temperature threshold
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("DHT reading failed\n");
        }
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}