#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "dht.h"
#include "architecture.h"
#include "thread_arch.h"
#include "event/thread.h"
#include "thread.h"
#include "ztimer.h"
#include <stdio.h>

#define DHT_GPIO_PIN 2
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)

static char stack[THREAD_STACKSIZE];

dht_t dev;

void *read_sensor(void *arg) {
    (void)arg;
    int16_t temperature, humidity;
    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                temperature / 10, temperature % 10, 
                humidity / 10, humidity % 10);
        }
        else {
            printf("Failed to read from sensor\n");
        }
        ztimer_sleep(ZTIMER_USEC, 5000000);  // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    printf("DHT11 reader example\n");

    if (dht_init(&dev, dht_params) != 0) {
        printf("Sensor initialization failed\n");
        return 1;
    }

    kernel_pid_t thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY, 0,
                                            read_sensor, NULL, "sensor_read");
    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    return 0;
}