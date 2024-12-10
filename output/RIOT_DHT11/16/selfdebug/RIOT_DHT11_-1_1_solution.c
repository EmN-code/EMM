#include "thread_arch.h"
#include "thread_config.h"
#include "thread_flags.h"
#include <stdio.h>
#include "xtimer.h"
#include "dht.h"
#include "esp_idf_gpio.h"

#define DHT_PIN 2
#define MEASURE_INTERVAL (5U * US_PER_SEC)

static dht_t dev;

static void *read_dht(void *arg) {
    (void)arg;
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C Humidity: %d.%d%%\n", temp / 10, temp % 10,
                   hum / 10, hum % 10);
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }
        xtimer_sleep(MEASURE_INTERVAL);
    }
    return NULL;
}

int main(void) {
    esp_idf_gpio_reset_pin(DHT_PIN);

    // Initialize the DHT11 device
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;

    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    // Start the thread to read from DHT11
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_dht, NULL, "dht_reader");

    return 0;
}