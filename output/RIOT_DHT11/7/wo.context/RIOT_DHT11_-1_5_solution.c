#include "xtimer.h"
#include "dht.h"
#include "dht_params.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11

int main(void) {
    dht_t dev;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    printf("DHT Example for ESP32
");

    if (dht_init(&dev, &dht_params[DHT_TYPE], DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT
");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT sensor
");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%
", temp / 10, temp % 10, hum / 10, hum % 10);
        }
        xtimer_periodic_wakeup(&last_wakeup, 5 * XTIMER_TICKS_PER_SEC);
    }
    return 0;
}