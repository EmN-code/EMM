#include <xtimer.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_B, 1)
#define TEMP_THRESHOLD 30  // Temperature threshold in °C

int main(void) {
    // Initialize the DHT11 sensor
    dht_t dev;
    dev.type = DHT11;
    dev.params.pin = DHT_PIN;

    int16_t temp, hum;
    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            temp /= 10;  // Convert temperature to Celsius

            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        } else {
            // Handle read error
        }

        xtimer_sleep(5);  // Wait for 5 seconds
    }
    return 0;
}
