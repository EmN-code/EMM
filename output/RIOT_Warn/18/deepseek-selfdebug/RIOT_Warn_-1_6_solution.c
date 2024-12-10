
#include <stdio.h>
#include "periph/gpio.h"
#include "periph/timer.h"
#include "dht.h"
#include "xtimer.h"

#define LED_PIN         GPIO_PIN(0, 13)  // Example GPIO pin for LED
#define DHT11_PIN       GPIO_PIN(0, 14)  // Example GPIO pin for DHT11
#define TEMP_THRESHOLD  30               // Temperature threshold in Celsius
#define INTERVAL        5                // Interval in seconds

static void read_dht11(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT11_PIN) != DHT_OK) {
        printf("DHT11 initialization failed\n");
        return;
    }

    if (dht_read(&dev, &temp, &hum) != DHT_OK) {
        printf("DHT11 read failed\n");
        return;
    }

    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
           temp / 10, temp % 10, hum / 10, hum % 10);

    if (temp / 10 > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        read_dht11();
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
