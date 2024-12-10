#include <thread_flags.h>
#include <stdio.h>
#include <dht.h>
#include <xtimer.h>

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2 for the DHT11
#define READ_INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    dht_params_t dht_params;

    dht_params.pin = DHT_PIN;
    dht_params.type = DHT11;

    if (dht_init(&dev, &dht_params) != DHT_OK) {
        printf("Error initializing DHT11\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Error reading DHT11 data\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        }

        xtimer_sleep(READ_INTERVAL / US_PER_SEC);
    }

    return 0;
}