#include <tim.h>
#include <thread_flags.h>
#include <thread.h>
#include <stdio.h>
#include <dht.h>

#define DHT_PIN 2
#define DHT_TYPE DHT11

#define SLEEP_TIME_S (5U)  // Sleep time in seconds

static dht_t dev;

void *dht_reading_thread(void *arg) {
    (void)arg;

    int16_t temperature, humidity;
    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10,
                                                        humidity / 10, humidity % 10);
        } else {
            printf("Error reading from DHT sensor\n");
        }

        xtimer_sleep(SLEEP_TIME_S);
    }

    return NULL;
}

int main(void) {
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT_TYPE;

    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    static char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, dht_reading_thread, NULL, "dht_thread");

    return 0;
}