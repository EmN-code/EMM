#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN     GPIO_PIN(0, 2)
#define DHT_TYPE    DHT11
#define INTERVAL    (5U * 1000000U)  // 5 seconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    
    // Initialize DHT sensor
    if (dht_init(&dev, DHT_TYPE) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }

    while (1) {
        // Read DHT sensor data
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C
", temp / 10, temp % 10);
            printf("Humidity: %d.%d%%
", hum / 10, hum % 10);
        } else {
            printf("Failed to read sensor data\n");
        }

        // Sleep for the defined interval
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }
    return 0;
}