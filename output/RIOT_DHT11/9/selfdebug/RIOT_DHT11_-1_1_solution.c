#include "thread_config.h"
#include "thread_arch.h"
#include "thread_flags.h"
#include "timex.h"
#include "ztimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)

static dht_t dht_sensor;
static dht_params_t dht_params = {
    .pin = DHT_PIN,
    .type = DHT11
};

void *read_dht(void *arg) {
    (void)arg;
    int16_t temperature, humidity;
    while (1) {
        if (dht_read(&dht_sensor, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return NULL;
}

int main(void) {
    dht_init(&dht_sensor, &dht_params);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_dht, NULL, "dht_thread");
    return 0;
}