#include <thread_config.h>
#include <thread_arch.h>
#include <event/timeout.h>

#include "dht.h"
#include "xtimer.h"

#define DHT_PIN GPIO_NUM_2
#define MEASURE_INTERVAL (5 * US_PER_SEC)

void *read_dht(void *arg) {
    int16_t temp, hum;
    dht_t dev;
    dht_params_t params;

    params.pin = DHT_PIN;
    params.type = DHT11;

    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize DHT11\n");
        return NULL;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read data from DHT11\n");
        }
        xtimer_sleep(MEASURE_INTERVAL / US_PER_SEC);
    }
    return NULL;
}

int main(void)
{
    /* Start to read temperature and humidity in a separate thread */
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  read_dht, NULL, "read_dht");

    /* Main thread can do other tasks or sleep, depending on the application */
    while (1) {
        thread_sleep();
    }
    return 0;
}