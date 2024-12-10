
#include "thread.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "event/thread.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2 on ESP32
#define SLEEP_INTERVAL (5 * 1000000UL)  // 5 seconds in microseconds

static dht_t dht_dev;

static void *dht_reader(void *arg)
{
    (void)arg;
    int16_t temp, hum;

    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == 0) {
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
    if (gpio_init(DHT_PIN, GPIO_OUT) < 0) {
        puts("Failed to initialize GPIO");
        return 1;
    }

    dht_params_t params = { .pin = DHT_PIN, .type = DHT11 };
    if (dht_init(&dht_dev, &params) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        dht_reader, NULL, "dht_reader"
    );

    return 0;
}
