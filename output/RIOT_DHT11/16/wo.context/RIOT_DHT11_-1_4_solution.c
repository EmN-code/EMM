#include <xtimer.h>
#include <stdio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2 on ESP32
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    
    if (dht_init(&dev, DHT_PIN) != 0) {
        printf("[Error] Could not initialize DHT sensor\n");
        return 1;
    }
    
    while (1) {
        xtimer_sleep(INTERVAL / US_PER_SEC);

        // Read data from DHT11
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("[Error] Could not read data from DHT sensor\n");
        } else {
            printf("Temperature: %d.%dC, Humidity: %d.%d%%\n", 
                temperature / 10, temperature % 10, 
                humidity / 10, humidity % 10);
        }
    }
    return 0;
}