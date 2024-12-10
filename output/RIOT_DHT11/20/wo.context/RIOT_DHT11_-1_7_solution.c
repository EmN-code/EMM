#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 2)
#define SLEEP_INTERVAL 5U

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Error: Could not initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                    temp / 10, temp % 10, 
                    hum / 10, hum % 10);
        } else {
            printf("Error: Could not read DHT sensor\n");
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}