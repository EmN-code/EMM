#include "xtimer.h"
#include "periph/gpio.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_B, 0)
#define THRESHOLD_TEMP 30

static dht_t dht_dev;

void read_dht_sensor(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
               temp / 10, temp % 10, hum / 10, hum % 10);
        if (temp / 10 > THRESHOLD_TEMP) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Error reading DHT sensor\n");
    }
}

int main(void) {
    // Initialize GPIOs
    gpio_init(GPIO_PIN(PORT_A, 0), GPIO_IN);
    gpio_init(GPIO_PIN(PORT_B, 0), GPIO_OUT);

    // Initialize the DHT11 sensor
    dht_dev.pin = GPIO_PIN(PORT_A, 0);

    while (1) {
        read_dht_sensor();
        xtimer_sleep(5);
    }
    return 0;
}