#include "timex.h"
#include "thread.h"
#include "periph/gpio.h"
#include "periph/timer.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define TIMER_INTERVAL 5

static dht_t dev;
static char stack[THREAD_STACKSIZE_MAIN];

void *read_dht_thread(void *arg) {
    (void)arg; // Suppress unused variable warning
    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }
        xtimer_sleep(TIMER_INTERVAL);
    }
    return NULL;
}

int main(void) {
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;

    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_dht_thread, NULL, "dht11_reader");
    return 0;
}