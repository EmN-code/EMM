#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "thread.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "msg.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "architecture.h"
#include "dht.h"

#define DHT_PIN 2
#define SLEEP_TIME 5

void *dht11_read_thread(void *arg) {
    (void) arg;
    dht_t dev;
    int16_t temperature, humidity;

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C\n", temperature / 10, temperature % 10);
            printf("Humidity: %d.%d%%\n", humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
        sleep(SLEEP_TIME);
    }
    return NULL;
}

int main(void) {
    char thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                   THREAD_CREATE_STACKTEST, dht11_read_thread, NULL, "dht11_reader");

    return 0;
}
