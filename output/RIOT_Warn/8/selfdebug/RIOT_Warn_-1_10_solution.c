#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN         GPIO_PIN(PORT_A, 6)
#define LED_PIN         GPIO_PIN(PORT_A, 7)
#define TEMP_THRESHOLD  30  // Temperature threshold in °C

static dht_t dev;

static void read_dht_sensor(void) {
    int16_t temp, hum;
    int res = dht_read(&dev, &temp, &hum);
    if (res == DHT_OK) {
        printf("DHT values - Temperature: %dC, Humidity: %d%%\n", temp / 10, hum / 10);
        if (temp >= TEMP_THRESHOLD * 10) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Failed to read DHT sensor\n");
    }
}

int main(void) {
    int res = dht_init(&dev, DHT_PIN, DHT11);
    if (res != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }
    gpio_init(LED_PIN, GPIO_OUT);
    
    while (1) {
        read_dht_sensor();
        xtimer_sleep(5);
    }
    return 0;
}