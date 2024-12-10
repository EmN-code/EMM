#include <xtimer.h>  // Include for xtimer, which is outdated for time-based operations
#include <stdio.h>   // Provides functionalities to output data
#include <periph/gpio.h>  // For GPIO operations
#include "dht.h"  // Include DHT sensor specific operations

#define DHT11_PIN GPIO_PIN(PORT_A, 8)  // Pin where DHT11 data pin is connected, correct this for your setup
#define LED_PIN GPIO_PIN(PORT_A, 9)  // Pin where LED is connected, correct this for your setup
#define TEMP_THRESHOLD 30  // Temperature threshold for LED triggering
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

static dht_t dev;
static gpio_t led_pin = LED_PIN;

void read_temperature(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) != DHT_OK) {
        puts("Failed to read from DHT sensor!");
        return;
    }
    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
    if (temp / 10 > TEMP_THRESHOLD) {
        gpio_set(led_pin);
    } else {
        gpio_clear(led_pin);
    }
}

int main(void) {
    if (dht_init(&dev, DHT11_PIN, DHT11) != DHT_OK) {
        puts("Failed to initialize DHT sensor!");
        return 1;
    }
    gpio_init(led_pin, GPIO_OUT);

    while (1) {
        read_temperature();
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}