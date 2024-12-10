#include <timex.h>
#include <thread_config.h>
#include <thread_arch.h>
#include <periph/gpio.h>
#include <net/nanocoap/cache.h>
#include <dht.h>

static dht_t dht11_dev;

void *thread_func(void *arg) {
    (void) arg;
    while(1) {
        xtimer_sleep(5);

        int16_t temperature, humidity;
        if (dht_read(&dht11_dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C\n", temperature / 10, temperature % 10);
            printf("Humidity: %d.%d%%\n", humidity / 10, humidity % 10);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }
    }
    return NULL;
}

int main(void) {
    dht_params_t params;
    params.pin = GPIO2;
    params.type = DHT11;

    if (dht_init(&dht11_dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    printf("DHT11 sensor initialized, starting read thread.\n");

    thread_t *thread = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST, thread_func, NULL, "dht_thread");

    if (thread == NULL) {
        printf("Failed to create thread\n");
        return 1;
    }

    return 0;
}