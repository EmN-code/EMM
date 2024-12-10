#include <dht.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DHT_PIN  (23)  // Define your DHT pin
#define WARNING_TEMP (30)  // Temperature threshold in Celsius
#define LED_PIN  (2)  // Define your LED pin

static dht_t dev;

void init_sensors(void) {
    const dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    dht_init(&dev, &params);
}

void read_temperature(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) != 0) {
        printf("Failed to read from DHT sensor\n");
        return;
    }
    printf("Temperature: %d.%d°C - Humidity: %d.%d%%\n", temp / 10, abs(temp % 10), hum / 10, hum % 10);

    if (temp / 10 >= WARNING_TEMP) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    init_sensors();
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        read_temperature();
        xtimer_sleep(5);
    }
    return 0;
}