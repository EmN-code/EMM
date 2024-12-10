
#include "periph/gpio.h"
#include "periph/timer.h"
#include "dht.h"
#include "xtimer.h"
#include <stdio.h>

#define DHT_PIN             GPIO_PIN(0, 12)  // GPIO12 on ESP32
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO2 on ESP32
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define INTERVAL            5                // Interval in seconds

static void read_dht11(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        puts("DHT11 initialization failed");
        return;
    }

    if (dht_read(&dev, &temp, &hum) != DHT_OK) {
        puts("DHT11 read failed");
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
