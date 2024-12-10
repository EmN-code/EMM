
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <periph/gpio.h>
#include <ztimer.h>

#define DHT_PIN            GPIO_PIN(0, 18)
#define LED_PIN            GPIO_PIN(0, 21)
#define TEMP_THRESHOLD     30
#define READ_INTERVAL      5000000  // 5 seconds in microseconds

typedef struct {
    gpio_t pin;
    int16_t temperature;
    int16_t humidity;
} dht_t;

static dht_t dht_dev = {
    .pin = DHT_PIN,
    .temperature = 0,
    .humidity = 0
};

static void read_dht(dht_t *dev) {
    // Simulated DHT11 read function
    dev->temperature = 25;  // Simulated temperature value
    dev->humidity = 50;      // Simulated humidity value
}

static void check_temperature(dht_t *dev, gpio_t led_pin) {
    if (dev->temperature > TEMP_THRESHOLD) {
        gpio_set(led_pin);
    } else {
        gpio_clear(led_pin);
    }
}

int main(void) {
    gpio_init(DHT_PIN, GPIO_OUT);
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        read_dht(&dht_dev);
        check_temperature(&dht_dev, LED_PIN);
        ztimer_sleep(ZTIMER_USEC, READ_INTERVAL);
    }

    return 0;
}
