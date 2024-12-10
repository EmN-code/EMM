#include <thread_arch.h>
#include <stdio.h>
#include <stdint.h>
#include "dht.h"
#include "thread.h"
#include "esp_idf_gpio_config.h"
#include "esp_idf_gpio_reset_pin.h"

#define DHT_PIN 2

static dht_t dht_dev;

void *read_dht(void *arg) {
    (void)arg; // Unused variable
    int16_t temperature, humidity;
    while (1) {
        if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            printf("Could not read data from DHT11\n");
        }
        thread_sleep(5);
    }
    return NULL;
}

int main(void) {
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;

    esp_idf_gpio_reset_pin(params.pin);

    if (dht_init(&dht_dev, &params) != 0) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    thread_create(NULL, 1024, THREAD_PRIORITY_MAIN - 1, 0, read_dht, NULL, "dht_reader");

    return 0;
}