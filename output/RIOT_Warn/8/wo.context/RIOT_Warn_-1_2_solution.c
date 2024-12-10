#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "xtimer.h"
#include "thread.h"
#include "timex.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 5) // Depending on your board layout
#define LED_PIN GPIO_PIN(0, 2) // Adjust according to your ESP32 setup
#define TEMPERATURE_THRESHOLD 30
#define INTERVAL (5U * US_PER_SEC) // 5 seconds

dht_t dev;
int main(void) {
    if (dht_init(&dev, &DHT_PIN) != 0) {
        printf("DHT initialization failed\n");
        return -1;
    }

    gpio_init(LED_PIN, GPIO_OUT);
    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read data from sensor\n");
        } else {
            printf("Temperature: %d°C - Humidity: %d%%\n", temp, hum);
            if (temp > TEMPERATURE_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Temperature exceeds threshold! LED turned on.\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature is normal. LED turned off.\n");
            }
        }
        xtimer_sleep(5);
    }
    return 0;
}