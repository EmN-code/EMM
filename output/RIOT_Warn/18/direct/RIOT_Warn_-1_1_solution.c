#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_TYPE DHT11
#define GPIO_PIN_DHT GPIO_PIN(0, 4) // Assuming GPIO 0, PIN 4 for DHT11
#define GPIO_PIN_LED GPIO_PIN(0, 2) // Assuming GPIO 0, PIN 2 for LED
#define THRESHOLD_TEMP 25 // Set your temperature threshold
#define INTERVAL (5LU * US_PER_SEC) // 5 seconds interval

static dht_t dev;

void read_sensor(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, abs(temp % 10), hum / 10, abs(hum % 10));

        if (temp / 10 > THRESHOLD_TEMP) {
            gpio_set(GPIO_PIN_LED);
            printf("Temperature threshold exceeded! LED ON\n");
        } else {
            gpio_clear(GPIO_PIN_LED);
            printf("Temperature normal. LED OFF\n");
        }
    }
    else {
        printf("Error reading DHT11 sensor.\n");
    }
}

int main(void) {
    // Initialize the DHT11 sensor
    if (dht_init(&dev, GPIO_PIN_DHT, DHT_TYPE) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    // Initialize the LED pin
    if (gpio_init(GPIO_PIN_LED, GPIO_OUT) < 0) {
        printf("Failed to initialize GPIO for LED\n");
        return 1;
    }

    printf("System initialized. Starting sensor read loop.\n");

    // Loop to read the sensor every 5 seconds
    while (1) {
        read_sensor();
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}