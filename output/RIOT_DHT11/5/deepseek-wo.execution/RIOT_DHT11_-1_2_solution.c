
#include "periph/gpio.h"
#include "event/thread.h"
#include "xtimer.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_TYPE            DHT11
#define SLEEP_INTERVAL      (5 * 1000000U)  // 5 seconds in microseconds

static dht_t dht_dev;

static void *dht_read_thread(void *arg)
{
    (void)arg;
    int16_t temp, hum;

    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == 0) {
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
    if (gpio_init(DHT_PIN, GPIO_OUT) < 0) {
        puts("Failed to initialize GPIO");
        return 1;
    }

    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE,
        .in_mode = GPIO_IN_PU
    };

    if (dht_init(&dht_dev, &dht_params) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    event_thread_t dht_thread;
    event_thread_create(&dht_thread, "dht_reader", dht_read_thread, NULL, EVENT_PRIO_MEDIUM);

    return 0;
}
