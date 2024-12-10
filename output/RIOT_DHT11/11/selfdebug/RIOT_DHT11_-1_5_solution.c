#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5U * 1000000U) // 5 seconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Properly initialize the DHT11 sensor on GPIO2
    if (dht_init(&dev, DHT_PIN, DHT11) == DHT_OK) {
        while (1) {
            if (dht_read(&dev, &temp, &hum) == DHT_OK) {
                printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                       temp / 10, temp % 10, hum / 10, hum % 10);
            } else {
                printf("Failed to read from sensor\n");
            }
            xtimer_usleep(INTERVAL);
        }
    } else {
        printf("Failed to initialize sensor\n");
    }
    return 0;
}