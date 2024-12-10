#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 1)
#define LED_PIN GPIO_PIN(PORT_A, 2)
#define TEMP_THRESHOLD 30
#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

void read_sensor_and_control_led(dht_t *dev) {
    int16_t temp, hum;
    
    if (dht_read(dev, &temp, &hum) != 0) {
        printf("Error reading data from DHT11\n");
        return;
    }
    
    printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temp / 10, abs(temp % 10), hum / 10, abs(hum % 10));
    
    if (temp / 10 > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
        printf("Warning: Temperature exceeds threshold!\n");
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    dht_t dev;
    dht_params_t params;

    params.pin = DHT_PIN;
    params.type = DHT11;

    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }
    
    gpio_init(LED_PIN, GPIO_OUT);
    
    while (1) {
        read_sensor_and_control_led(&dev);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}