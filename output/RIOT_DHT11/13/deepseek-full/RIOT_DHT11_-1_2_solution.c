
#include "thread.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "event/thread.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void read_dht(void)
{
    int16_t temperature, humidity;
    dht_t dev;

    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor");
        return;
    }

    if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
        puts("Failed to read from DHT11 sensor");
        return;
    }

    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
           temperature / 10, temperature % 10,
           humidity / 10, humidity % 10);
}

static void *dht_thread(void *arg)
{
    (void)arg;

    while (1) {
        read_dht();
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_thread");

    return 0;
}
