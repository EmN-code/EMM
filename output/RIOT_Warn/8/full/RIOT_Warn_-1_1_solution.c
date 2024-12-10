#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT11_PIN GPIO_PIN(PORT_A, 0)   // GPIO pin for DHT11
#define LED_PIN GPIO_PIN(PORT_B, 1)     // GPIO pin for LED
#define TEMP_THRESHOLD 30               // Temperature threshold in °C

static dht_t dev;

int main(void)
{
    int16_t temp, hum;
    dht_params_t params;
    params.pin = DHT11_PIN;
    params.type = DHT11;

    // Initialize the DHT11 sensor
    if (dht_init(&dev, &params) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    // Initialize the LED
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        puts("Failed to initialize LED");
        return 1;
    }

    while (1) {
        int res = dht_read(&dev, &temp, &hum);

        if (res == DHT_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);

            // Turn on the LED if the temperature exceeds the threshold
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            puts("Failed to read from DHT sensor");
        }

        // Wait for 5 seconds before next reading
        xtimer_sleep(5);
    }

    return 0;
}