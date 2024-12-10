#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>
#include <stdio.h>

#define DHT11_PIN GPIO_PIN(0, 2)  // GPIO pin where the DHT11 is connected
#define READ_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

int main(void) {
    dht_t dev;
    int16_t temperature,
            humidity;

    if (dht_init(&dev, DHT11_PIN) != DHT_OK) {
        printf("Error initializing DHT11\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Could not read data from DHT11\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        }
        xtimer_usleep(READ_INTERVAL);
    }

    return 0;
}