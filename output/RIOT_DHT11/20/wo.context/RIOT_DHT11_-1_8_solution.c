#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 2) // Correct pin definition for GPIO2
#define INTERVAL (5U * US_PER_SEC) // 5-second interval

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read data from DHT sensor\n");
        }
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}