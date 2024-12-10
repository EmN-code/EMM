#include <xtimer.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PARAM    DHT_PARAM_0  // Default parameters defined in periph_conf.h
#define INTERVAL     5            // Interval in seconds
#define TEMP_THRESH  25           // Temperature threshold in degree Celsius
#define LED_PIN      GPIO_PIN(PORT_A, 5) // Example LED pin configuration

int main(void) {
    // Initialize GPIO pin for LED
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    // Initialize DHT sensor
    dht_t dev;
    if (dht_init(&dev, &DHT_PARAM) != DHT_OK) {
        puts("Error initializing DHT sensor");
        return 1;
    }

    int16_t temp, hum;

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Error reading DHT sensor");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESH) {
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN); // Turn off LED
            }
        }
        xtimer_sleep(INTERVAL);
    }

    return 0;  // Although never reached
}