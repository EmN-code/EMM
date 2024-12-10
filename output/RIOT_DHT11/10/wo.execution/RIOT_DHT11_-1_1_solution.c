#include "thread_config.h"
#include "thread_arch.h"
#include "thread_flags.h"
#include "event/timeout.h"
#include <stdio.h>
#include "dht.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5U * US_PER_SEC)

void *read_dht(void *arg) {
    dht_t dev;
    dht_params_t params = {
        .pin      = DHT_PIN,
        .type     = DHT11
    };
    
    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return NULL;
    }

    while (1) {
        int16_t temperature, humidity;
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        } else {
            printf("Failed to read DHT sensor\n");
        }
        xtimer_sleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid = thread_create(
        stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST, read_dht, NULL, "read_dht"
    );
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
    }
    return 0;
}
