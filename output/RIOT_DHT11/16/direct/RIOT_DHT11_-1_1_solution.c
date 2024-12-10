#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define DHT11_TYPE DHT11

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;

    if (dht_init(&dev, DHT11_PIN, DHT11_TYPE) != DHT_OK) {
        printf("Error initializing DHT11\n");
        return 1;
    }

    printf("DHT11 successfully initialized on GPIO2\n");

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temperature / 10, temperature % 10, 
                   humidity / 10, humidity % 10);
        } else {
            printf("Error reading data from DHT11\n");
        }
        xtimer_sleep(5);
    }

    return 0;
}