#include "thread.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"

#define DHT11_PIN 2
#define MAIN_THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)

dht_t dev;

char stack[MAIN_THREAD_STACKSIZE];

void *read_dht11(void *arg) {
    (void)arg;
    int16_t temperature, humidity;

    while(1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                temperature / 10, temperature % 10,
                humidity / 10, humidity % 10);
        } else {
            printf("Error reading from DHT11 sensor\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    dht_params_t params;
    params.pin = DHT11_PIN;
    params.type = DHT11;

    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_dht11, NULL, "read_dht11");

    while(1) {
        /* Keep the main thread active */
    }

    return 0;
}