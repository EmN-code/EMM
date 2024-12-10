#include <ztimer.h>
#include <thread.h>
#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define DHT11_GPIO 2
#define MAIN_QUEUE_SIZE 8

static kernel_pid_t dht_thread_pid;
char dht_thread_stack[THREAD_STACKSIZE_MAIN];

dht_t dev;

void *dht_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;
    while(1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return NULL;
}

int main(void) {
    dht_params_t my_dht_params;
    my_dht_params.pin = DHT11_GPIO;
    
    if (dht_init(&dev, &my_dht_params) == 0) {
        printf("DHT11 sensor initialized\n");
    } else {
        printf("Failed to initialize DHT11 sensor\n");
    }

    dht_thread_pid = thread_create(dht_thread_stack, sizeof(dht_thread_stack), 
                                   THREAD_PRIORITY_MAIN - 1, 0, 
                                   dht_thread, NULL, "dht_thread");

    return 0;
}