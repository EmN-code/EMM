#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 0) // Adjust based on your board
#define LED_PIN GPIO_PIN(0, 1) // Adjust based on your board
#define TEMP_THRESHOLD 30
#define MEASUREMENT_INTERVAL (5U * US_PER_SEC)

static dht_t dht_dev;

void measure(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d °C, Humidity: %d.%d %%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        if ((temp / 10) > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        }
        else {
            gpio_clear(LED_PIN);
        }
    } else {
        puts("Failed to read from DHT sensor");
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_init(&dht_dev, DHT_PIN, DHT11);

    while (1) {
        measure();
        xtimer_usleep(MEASUREMENT_INTERVAL);
    }

    return 0;
}