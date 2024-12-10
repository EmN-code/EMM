#include <ztimer.h>
#include <dht.h>
#include <stdio.h>

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO2 is often referred to as (0, 2) in RIOT
#define DHT_TYPE DHT11
#define READ_INTERVAL (5U * US_PER_SEC)  // Interval between reads: 5 seconds

int main(void) {
    dht_t dev;
    dht_params_t params;
    int16_t temp, hum;

    params.pin = DHT_PIN;
    params.type = DHT_TYPE;

    // Initialize the DHT sensor
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp/10, temp%10, hum/10, hum%10);
        } else {
            printf("Error reading from DHT sensor\n");
        }
        
        ztimer_sleep(ZTIMER_MSEC, READ_INTERVAL);
    }

    return 0;
}