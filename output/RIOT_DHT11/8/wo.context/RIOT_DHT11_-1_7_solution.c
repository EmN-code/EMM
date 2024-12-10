#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"
#include "dht.h"
#include "dht_params.h"

#define DHT_TYPE           DHT11
#define DHT_PIN            GPIO_PIN(0, 2)  // GPIO 2
#define SLEEP_INTERVAL     (5U * US_PER_SEC)

int main(void) {
    dht_t dev;
    gpio_init(DHT_PIN, GPIO_IN);

    if (dht_init(&dev, &dht_params[0]) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    int16_t temp, hum;

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }

        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}