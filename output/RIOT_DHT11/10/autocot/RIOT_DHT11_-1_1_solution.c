#include "thread_flags.h"
#include "thread_arch.h"
#include "timer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11

void read_dht11(void) {
    int16_t temp, hum;
    dht_t dev;
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE
    };
    
    if (dht_init(&dev, &params) != 0) {
        printf("DHT init failed\n");
        return;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
                    temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11\n");
        }
        
        xtimer_sleep(5);
    }
}

int main(void) {
    printf("Initializing DHT11 Reader\n");
    read_dht11();
    return 0;
}