
#include "thread.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "dht_params.h"
#include "event/thread.h"
#include "dht.h"

#include <stdio.h>
#include <xtimer.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static void *dht_thread(void *arg)
{
    (void)arg;
    int16_t temp, hum;

    while (1) {
        if (dht_read(NULL, &temp, &hum) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
        } else {
            puts("Failed to read from DHT sensor");
        }

        xtimer_usleep(SLEEP_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    puts("DHT11 Sensor Reading Example");

    // Initialize the DHT11 sensor
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    dht_t dev;
    if (dht_init(&dev, &params) != 0) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    // Create a thread for reading the DHT11 sensor
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0, dht_thread, NULL, "dht_thread");

    return 0;
}
