#include "xtimer.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "dht.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(0, 4)  // Use appropriate pin for DHT data
#define LED_PIN GPIO_PIN(0, 5)  // Pin for the LED
#define TEMP_THRESHOLD 30       // Temperature threshold in Celsius for the LED warning

int main(void) {
    // Initialize GPIO
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN); // Make sure LED is off initially

    // Configure DHT parameters
    dht_t dev;
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT_TYPE_DHT11;

    if (dht_init(&dev, &params) != DHT_OK) {
        printf("DHT sensor initialization failed\n");
        return 1;
    }

    int16_t temperature, humidity;

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
            
            if ((temperature / 10) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED if temperature exceeds threshold
            } else {
                gpio_clear(LED_PIN); // Else, make sure LED is off
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        
        xtimer_sleep(5); // Wait for 5 seconds before reading again
    }
    return 0;
}
