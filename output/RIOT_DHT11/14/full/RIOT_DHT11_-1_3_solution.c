#include "dht_params.h"
#include "thread.h"
#include "dht.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include <stdio.h>

#define DHT_PIN 2

static char stack[THREAD_STACKSIZE_MAIN];

void *dht_thread(void *arg) {
    dht_t dev;
    int16_t temp, hum;
    
    if (dht_init(&dev, &dht_params[0]) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return NULL;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                    temp / 10, temp % 10, hum / 10, hum % 10);
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    puts("RIOT application for DHT11 reading on ESP32\n");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 
                  THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_reader");
    
    while (1) {
        // Keep main alive
    }
    return 0;
}
