#include "ztimer.h"
#include "xtimer.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/timeout.h"
#include "event/thread.h"
#include "event/periodic.h"
#include "thread_config.h"
#include "thread.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "dht.h"
#include "msg.h"

#include <stdio.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define THREAD_STACK_SIZE THREAD_STACKSIZE_MAIN
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACK_SIZE];

void *read_dht(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        printf("DHT init failed\n");
        return NULL;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Could not read data from DHT sensor\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    puts("Starting DHT11 sensor reading on ESP32");

    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY,
                                     THREAD_CREATE_STACKTEST, read_dht, NULL,
                                     "dht11");
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Thread creation failed\n");
        return 1;
    }

    return 0;
}