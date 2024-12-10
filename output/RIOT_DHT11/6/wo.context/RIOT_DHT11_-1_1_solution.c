#include "thread_config.h"
#include "thread_arch.h"
#include "thread_flags.h"
#include "net/nanocoap/cache.h"
#include "architecture_arch.h"
#include "mqtt_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5U * US_PER_SEC)  // 5 seconds

static dht_t dev;

void *dht_read_thread(void *arg) {
    (void)arg;

    int16_t temperature, humidity;

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            printf("Could not read data from sensor\n");
        }

        xtimer_sleep(SLEEP_INTERVAL);
    }

    return NULL;
}

int main(void) {
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;

    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    kernel_pid_t pid = thread_create(NULL,                 // Stack memory (NULL = prepared stack)
                                     THREAD_SIZE_DEFAULT,  // Stack size
                                     THREAD_PRIORITY_MAIN, // Thread priority
                                     THREAD_CREATE_STACKTEST,  // Create flags
                                     dht_read_thread,     // Thread function
                                     NULL,                 // Argument for thread function
                                     "dht_reader");

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Thread creation failed\n");
        return 1;
    }

    // Let the main thread sleep indefinitely
    while (1) {
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}