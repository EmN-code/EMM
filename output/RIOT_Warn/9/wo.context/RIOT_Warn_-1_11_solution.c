#include <periph/gpio.h>
#include <periph/uart.h>
#include <stdio.h>
#include <xtimer.h>
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 1)  // Adjust as per your board's configuration
#define DHT_TYPE DHT11          // Assuming DHT11 is defined elsewhere
#define LED_PIN GPIO_PIN(0, 2)  // Adjust as per your board's configuration
#define TEMP_THRESHOLD 30       // Temperature threshold to turn on LED

int main(void) {
    // Initialize the DHT sensor
    dht_t dev;
    dht_init(&dev, DHT_PIN, DHT_TYPE);

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d degC, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN); // Turn off LED
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_sleep(5);  // Wait 5 seconds before next read cycle
    }
    return 0;
}