#include <xtimer.h>
#include <stdio.h>
#include <dht.h>

#define DHT_PIN GPIO2
#define INTERVAL (5 * XTIMER_SEC)

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    
    dht_init(&dev);

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        
        xtimer_sleep(INTERVAL);
    }
    return 0;
}