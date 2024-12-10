#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "dht_params.h"
#include "dht.h"
#include <stdio.h>

#define TEMP_THRESHOLD 30  // Temperature threshold for LED warning
#define DHT_PIN GPIO_PIN(PORT_X, X)  // Replace PORT_X and X with actual values
#define LED_PIN GPIO_PIN(PORT_Y, Y)  // Replace PORT_Y and Y with actual values

int main(void) {
    // Initialize the GPIO pin for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the DHT sensor
    dht_t dev;
    dev.pin = DHT_PIN;
    dev.type = DHT11;
    dht_init(&dev);

    while (1) {
        int16_t temperature, humidity;
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d%%\n", temperature / 10, temperature % 10, humidity);
            // Check if temperature exceeds threshold
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_sleep(5);  // Sleep for 5 seconds
    }
    return 0;
}