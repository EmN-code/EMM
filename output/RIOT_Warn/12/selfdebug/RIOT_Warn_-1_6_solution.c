#include<xtimer.h>  
#include <stdio.h>
#include "dht.h"
#include "periph/gpio.h"

#define DHT11_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_A, 1)
#define TEMP_THRESHOLD 30

dht_t dev;

void read_dht11(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);
        if (temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    }
    else {
        printf("Failed to read from DHT11 sensor\n");
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_params_t params;
    params.pin = DHT11_PIN;
    params.type = DHT_TYPE_DHT11;
    dht_init(&dev, &params);

   while (1) {
        read_dht11();
        xtimer_sleep(5);
    }
    return 0;
}