#include <stdio.h>
#include "xtimer.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 4) // Set the GPIO pin where the DHT11 is connected
#define LED_PIN GPIO_PIN(0, 5) // Set the GPIO pin where the LED is connected
#define TEMP_THRESHOLD 30 // Temperature threshold in °C
#define READ_INTERVAL (5U * US_PER_SEC) // 5 seconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    gpio_init(LED_PIN, GPIO_OUT);
    dht_params_t params;
    params.pin = DHT_PIN;

    if (dht_init(&dev, &params) == 0) {
        printf("DHT11 initialized successfully\n");
    } else {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp/10, hum/10);
            // Check if temperature exceeds the threshold
            if ((temp/10) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: Temperature exceeded threshold!\n");
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read data from DHT11\n");
        }
        xtimer_sleep(5);
    }
    return 0;
}