#include <xtimer.h>
#include <stdio.h>
#include "dht.h"
#include "periph/gpio.h"

#define DHT11_PIN GPIO_PIN(0, 2)  // Pin 2 on GPIO port 0
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

static dht_t dht_dev;
static dht_params_t dht_params = {
    .pin = DHT11_PIN,
    .type = DHT_TYPE_DHT11
};

void read_sensor(void) {
    int16_t temperature, humidity;
    if (dht_read(&dht_dev, &temperature, &humidity) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temperature / 10, temperature % 10,
               humidity / 10, humidity % 10);
    } else {
        printf("Error reading from DHT11 sensor\n");
    }
}

int main(void) {
    printf("Initializing DHT11 on GPIO_PIN(0,2)\n");
    if (dht_init(&dht_dev, &dht_params) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }
    
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        read_sensor();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }
    
    return 0;
}