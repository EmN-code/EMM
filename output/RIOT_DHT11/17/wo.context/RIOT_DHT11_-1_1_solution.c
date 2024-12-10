#include <xtimer.h>
#include <timex.h>
#include <stdio.h>
#include <dht.h>

#define DHT_PIN     GPIO_PIN(0, 2)
#define DHT_TYPE    DHT11
#define INTERVAL    (5U * US_PER_SEC) // 5 seconds

static dht_t dht_dev;

void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
               temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        printf("Failed to read from DHT sensor\n");
    }
}

int main(void) {
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT_TYPE;
    
    if (dht_init(&dht_dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        read_dht();
        xtimer_sleep(INTERVAL);
    }

    return 0;
}