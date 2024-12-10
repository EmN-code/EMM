#include "thread_arch.h"
#include "thread.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "architecture.h"
#include <stdio.h>

#define DHT_PIN 2

static int16_t temperature, humidity;

void *read_dht(void *arg) {
    (void) arg;  // unused
    
    while (1) {
        if (dht_read(DHT_PIN, &temperature, &humidity) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        ztimer_sleep(ztimer_usec_from_sec(5));
    }

    return NULL;
}

int main(void) {
    dht_init();
    
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_dht, NULL, "dht_thread");

    while(1) {
        // Main loop does nothing, all work handled by thread
    }

    return 0;
}