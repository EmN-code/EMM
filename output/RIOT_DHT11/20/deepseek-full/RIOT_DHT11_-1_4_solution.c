
#include "thread.h"
#include "periph/gpio.h"
#include "xtimer.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static dht_t dht_dev;

void read_dht_sensor(void)
{
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
        } else {
            puts("Failed to read from DHT sensor");
        }
        xtimer_usleep(SLEEP_INTERVAL);
    }
}

int main(void)
{
    if (gpio_init(DHT_PIN, GPIO_OUT) < 0) {
        puts("Failed to initialize GPIO");
        return 1;
    }

    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11
    };

    if (dht_init(&dht_dev, &dht_params) != 0) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        read_dht_sensor,
        NULL, "dht_reader"
    );

    return 0;
}
