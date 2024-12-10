#include "ztimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdatomic.h"

dht_t dht_dev;

void *dht_read_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp/10, temp%10, hum/10, hum%10);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    dht_params_t params;
    params.pin = GPIO_PIN(PORT_A, 2);
    params.type = DHT11;
    
    if (dht_init(&dht_dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }
    
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  dht_read_thread,
                  NULL, "dht_read_thread");
    
    return 0;
}