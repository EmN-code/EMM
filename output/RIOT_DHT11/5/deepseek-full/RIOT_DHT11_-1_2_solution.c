
#include "periph/i2c.h"
#include "thread.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "event/thread.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_TYPE            DHT11
#define SLEEP_INTERVAL      (5 * US_PER_SEC)

static dht_t dht_dev;

static void read_dht(void)
{
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read from DHT sensor");
    }
}

static void *dht_thread(void *arg)
{
    (void)arg;
    while (1) {
        read_dht();
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void)
{
    if (dht_init(&dht_dev, DHT_TYPE, DHT_PIN) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_thread");

    return 0;
}
