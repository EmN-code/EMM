#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(PORT_A, 0)  // Assume port and pin are configured properly
#define LED_PIN GPIO_PIN(PORT_B, 1)
#define THRESHOLD_TEMP 30 // Define the temperature threshold

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    int16_t temp, hum;
    dht_t dev;
    dht_params_t params;
    params.pin = DHT_PIN;

    if (dht_init(&dev, &params) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor\n");
        return -1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);
            if (temp > THRESHOLD_TEMP) {
                gpio_set(LED_PIN); // Turn on LED
            } else {
                gpio_clear(LED_PIN); // Turn off LED
            }
        } else {
            puts("Failed to read from DHT11 sensor\n");
        }
        xtimer_sleep(5);  // Wait for 5 seconds
    }
    return 0;
}