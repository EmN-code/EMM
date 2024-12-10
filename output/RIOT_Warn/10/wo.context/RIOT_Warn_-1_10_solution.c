#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 7)
#define LED_PIN GPIO_PIN(PORT_A, 6)
#define TEMP_THRESHOLD 30

void read_dht_and_control_led(void) {
    dht_t dev;
    int16_t temp, hum;
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;

    dht_init(&params, &dev);

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading DHT11\n");
        } else {
            printf("Temp: %d.%d°C, Hum: %d.%d\%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    read_dht_and_control_led();
    return 0;
}