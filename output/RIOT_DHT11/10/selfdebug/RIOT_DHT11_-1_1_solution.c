#include "timex.h"
#include "thread.h"
#include "stdio.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO2
#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)

void *read_dht(void *arg) {
    (void)arg; // suppress unused parameter warning
    dht_t dev;
    const dht_params_t params = { .pin = DHT_PIN, .type = DHT11 };
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return NULL;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read DHT11\n");
        }
        xtimer_sleep(5); // 5-second interval
    }

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_dht, NULL, "dht_reader");

    return 0;
}