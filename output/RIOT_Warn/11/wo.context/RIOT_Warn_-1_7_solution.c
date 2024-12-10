#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 4)  // Assuming pin 4 of port 0 is used for DHT11
#define LED_PIN GPIO_PIN(0, 5)  // Assuming pin 5 of port 0 is used for LED
#define THRESHOLD_TEMP 30       // Temperature threshold for warning (30°C)
#define SLEEP_INTERVAL (5U * US_PER_SEC) // 5 seconds

static dht_t dev;

void check_temperature(void) {
    int16_t temp, hum;
    int res = dht_read(&dev, &temp, &hum);
    if (res == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
               temp / 10, temp % 10, 
               hum / 10, hum % 10);
        if (temp >= THRESHOLD_TEMP * 10) {
            gpio_set(LED_PIN);
            printf("Warning: Temperature exceeds threshold!\n");
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Error reading DHT sensor\n");
    }
}

int main(void) {
    // Initialize DHT11 sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        check_temperature();
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }

    return 0;
}