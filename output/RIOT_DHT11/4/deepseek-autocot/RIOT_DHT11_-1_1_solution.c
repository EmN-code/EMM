
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN             GPIO_PIN(0, 2)  // GPIO 2 on ESP32
#define SLEEP_INTERVAL      (5 * 1000000U)  // 5 seconds in microseconds

static dht_t dht_dev;

static void *dht_reader(void *arg)
{
    (void)arg;

    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11,
    };

    if (dht_init(&dht_dev, &dht_params) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor");
        return NULL;
    }

    int16_t temp, hum;

    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            puts("Failed to read data from DHT11 sensor");
        }

        xtimer_usleep(SLEEP_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    puts("Starting DHT11 reader...");

    thread_create(
        thread_stack(dht_reader_stack),
        sizeof(dht_reader_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        dht_reader,
        NULL,
        "dht_reader"
    );

    return 0;
}
