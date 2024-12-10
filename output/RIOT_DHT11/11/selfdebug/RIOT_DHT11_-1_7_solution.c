#include <xtimer.h>
#include <stdio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 2) // GPIO pin where the DHT11 is connected
#define INTERVAL (5U * US_PER_SEC) // 5-second interval

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }

    // Loop to read temperature and humidity every five seconds
    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %.1f°C, Humidity: %.1f%%RH\n", temp / 10.0, hum / 10.0);
        }
        else {
            printf("Failed to read from DHT sensor\n");
        }

        xtimer_sleep(INTERVAL);
    }
    return 0;
}