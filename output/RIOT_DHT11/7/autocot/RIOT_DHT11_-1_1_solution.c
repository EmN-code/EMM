#include "thread_arch.h"
#include "thread.h"
#include "periph/gpio.h"
#include "xtimer.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5U * US_PER_SEC)

static dht_t dht_dev;
static dht_params_t dht_params = {
    .pin = DHT_PIN,
    .type = DHT11
};

void *dht_thread(void *arg) {
    (void)arg;
    int16_t temp = 0, hum = 0;
    dht_init(&dht_dev, &dht_params);
    
    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == 0) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp / 10, hum / 10);
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    thread_create(dht_thread_stack, sizeof(dht_thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_thread");
    
    return 0;
}