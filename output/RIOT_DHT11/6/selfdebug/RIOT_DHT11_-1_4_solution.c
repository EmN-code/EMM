#include <ztimer.h>
#include <thread.h>
#include <stdio.h>
#include <dht.h>

#define DHT11_PIN GPIO_PIN(0, 2)
#define INTERVAL (ZTIMER_USEC_BASE * 5)

char stack[THREAD_STACKSIZE_MAIN];

void *sensor_thread(void *arg) {
    (void)arg;
    dht_params_t dht_params;
    dht_params.pin = DHT11_PIN;
    dht_params.type = DHT11;

    if (dht_init(&dht_params) != 0) {
        puts("Failed to initialize DHT11 sensor");
        return NULL;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dht_params, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT11\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        }
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN-1,
                  THREAD_CREATE_STACKTEST, sensor_thread, NULL, "sensor");
    return 0;
}