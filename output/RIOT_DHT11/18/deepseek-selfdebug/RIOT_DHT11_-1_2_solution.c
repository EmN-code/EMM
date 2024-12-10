
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include "thread.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11
#define SLEEP_INTERVAL (5 * 1000 * 1000U) /* 5 seconds */

static void *dht_thread(void *arg)
{
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    while (1) {
        if (dht_read(&dev, DHT_TYPE, DHT_PIN, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            puts("Failed to read from DHT sensor");
        }
        xtimer_usleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void)
{
    dht_t dev;
    if (dht_init(&dev, DHT_TYPE, DHT_PIN) == DHT_OK) {
        puts("DHT sensor initialized");
    } else {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    thread_create(
        thread_stack(NULL, THREAD_STACKSIZE_DEFAULT),
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        dht_thread,
        NULL,
        "dht_thread"
    );

    return 0;
}
