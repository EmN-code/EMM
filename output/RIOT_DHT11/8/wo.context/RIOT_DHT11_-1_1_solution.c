#include "net/nanocoap/cache.h"
#include "event/timeout.h"
#include "event/periodic.h"
#include "architecture_arch.h"

#include <stdio.h>

#define DHT_PIN GPIO_NUM_2

static dht_t dev;

void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == 0) {
        printf("Temperature: %0.1f C, Humidity: %0.1f %%\n", temp/10.0, hum/10.0);
    } else {
        printf("Error reading DHT11\n");
    }
}

void timer_callback(void *arg) {
    (void)arg;
    read_dht();
}

int main(void) {
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;
    
    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    // Set a timer for 5000 ms (5 seconds)
    timer_set(TIMER_0, TIMER_CHANNEL_0, 5000);

    while (1) {
        // In real RIOT application, you should use a low power sleep or idling
    }

    return 0;
}