
#include "thread.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "dht_params.h"
#include "event/thread.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11
#define SLEEP_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static dht_t dht_dev;
static int16_t temperature, humidity;

void dht_read_thread(void) {
    while (1) {
        if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            puts("Failed to read from DHT sensor");
        }
        xtimer_usleep(SLEEP_INTERVAL);
    }
}

int main(void) {
    if (gpio_init(DHT_PIN, GPIO_OUT) < 0) {
        puts("Failed to initialize GPIO");
        return 1;
    }

    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE
    };

    if (dht_init(&dht_dev, &dht_params) != 0) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    thread_t *dht_thread;
    thread_create(
        (char *)"dht_thread",
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        dht_read_thread,
        NULL,
        "dht_thread"
    );

    return 0;
}
