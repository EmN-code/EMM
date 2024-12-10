#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include "dht.h"
#include "gpio.h"

#define DHT_PIN GPIO_PIN(PORT_A, 10)  // Example pin, adjust as necessary
#define LED_PIN GPIO_PIN(PORT_B, 0)  // Example pin, adjust as necessary
#define TEMP_THRESHOLD 30  // Temperature threshold in degrees Celsius
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

static dht_t dev;

int main(void) {
    // Initialize DHT sensor
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    // Initialize GPIO for LED control
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Failed to read data from DHT sensor");
        } else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp / 10, hum / 10);
            if ((temp / 10) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(INTERVAL / US_PER_SEC);  // Sleep for the defined interval
    }

    return 0;
}