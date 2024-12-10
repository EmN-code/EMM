#include "thread_arch.h"
#include "thread_config.h"
#include "event/timeout.h"
#include "dht.h"
#include "esp_idf_gpio_config.h"
#include <stdio.h>

#define DHT_PIN 2

static dht_t dev;
static dht_params_t params = {
    .pin = DHT_PIN,
    .type = DHT11,
};

void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        printf("Failed to read from DHT11\n");
    }
}

void *dht_thread(void *arg) {
    (void)arg;
    dht_init(&dev, &params);
    while (1) {
        read_dht();
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    puts("Starting DHT11 reader on ESP32\n");
    thread_create(dht_thread, NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 
                  THREAD_CREATE_STACKTEST, "dht_thread");
    return 0;
}