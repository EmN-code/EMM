#include "xtimer.h" 
#include "ztimer.h" 
#include "stdio.h" 
#include "stdlib.h" 
#include "stdbool.h" 
#include "stdint.h" 
#include "stddef.h" 
#include "stdatomic.h" 
#include "sys/socket.h" 
#include "string.h" 
#include "periph/gpio.h" 
#include "dht.h" 
#include "dht_params.h"
#include "esp_system.h"

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2
#define DHT_TYPE DHT11

void read_dht(void) {
    dht_t dev;
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT_TYPE;

    if (dht_init(&dev, &params) != 0) {
        printf("DHT init failed\n");
        return;
    }

    while (1) {
        int16_t temperature, humidity;
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temperature / 10, 
                   temperature % 10,
                   humidity / 10, 
                   humidity % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    read_dht();
    return 0;
}