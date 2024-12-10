#include <xtimer/xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dht/dht.h>
#include <msg/msg.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define THREAD_PRIORITY_MAIN THREAD_PRIORITY_MAIN
#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_DEFAULT)

dht_t dev;

void *dht_thread(void *arg) {
    (void)arg;
    int16_t temp;
    int16_t hum;
    while(true) {
        if(dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp/10, temp%10, hum/10, hum%10);
        } else {
            printf("Failed to read data from DHT sensor.\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    gpio_init(DHT_PIN, GPIO_IN);
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    dht_init(&dev, &params);
    
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN, 0, dht_thread, NULL, "dht_thread");
    
    return 0;
}