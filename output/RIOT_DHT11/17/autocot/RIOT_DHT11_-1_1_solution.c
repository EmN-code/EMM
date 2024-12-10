#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <esp_idf_gpio.h>

#define DHT_PIN GPIO_NUM_2
#define READ_INTERVAL 5U

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();
    dht_t dev;
    dht_params_t params = {
        .type = DHT_TYPE_DHT11,
        .pin = DHT_PIN
    };

    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    while (1) {
        int16_t temperature, humidity;
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT11\n");
        }
        xtimer_periodic_wakeup(&last_wakeup, READ_INTERVAL * US_PER_SEC);
    }

    return 0;
}