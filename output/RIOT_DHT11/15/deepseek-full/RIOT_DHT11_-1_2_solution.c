
#include "thread.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "event/thread.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static dht_t dht_dev;

static void *dht_read_thread(void *arg)
{
    (void)arg;
    int16_t temperature, humidity;

    while (1) {
        if (dht_read(&dht_dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        xtimer_usleep(SLEEP_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    if (gpio_init(DHT_PIN, GPIO_OUT) < 0) {
        printf("Failed to initialize GPIO\n");
        return 1;
    }

    dht_dev.pin = DHT_PIN;
    dht_dev.type = DHT11;

    if (dht_init(&dht_dev) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        dht_read_thread, NULL, "dht_read");

    return 0;
}
