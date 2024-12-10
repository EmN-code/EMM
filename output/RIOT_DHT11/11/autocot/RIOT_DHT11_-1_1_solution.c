#include <thread_config.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread_arch.h>
#include <dht_params_t>
#include <dht_t>

static dht_t dht_dev;

void *dht_reading_thread(void *arg) {
    (void)arg;

    int16_t temperature, humidity;
    while (1) {
        if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d Humidity: %d\n", temperature, humidity);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        thread_sleep(5);
    }
    return NULL;
}

int main(void) {
    dht_params_t dht_params = {
        .pin = GPIO_NUM_2,
        .type = DHT_TYPE_DHT11
    };

    if (dht_init(&dht_dev, &dht_params) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    thread_t dht_thread_handle;
    char stack[THREAD_STACKSIZE_MAIN];

    if (thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                      dht_reading_thread, NULL, "DHT Thread") <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    return 0;
}