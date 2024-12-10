#include <stdio.h>
#include <xtimer.h>
#include "dht.h"
#include "thread.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 4)     // Specify the appropriate GPIO pin
#define LED_PIN GPIO_PIN(0, 5)     // Specify the GPIO pin connected to the LED
#define TEMP_THRESHOLD 30          // Temperature threshold

static dht_t dev;

void init_sensors(void) {
    if (dht_init(&dev, DHT11, DHT_PIN) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor");
    }
    else {
        puts("DHT11 sensor initialized");
    }
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);
}

void check_temperature(void) {
    int16_t temp, hum; // Temperature and humidity
    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        puts("Failed to read from DHT sensor");
    }
}

int main(void) {
    init_sensors();

    while (1) {
        check_temperature();
        xtimer_sleep(5);
    }

    return 0;
}