#include <xtimer.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)    // GPIO 2
#define SLEEP_INTERVAL (5U * US_PER_SEC) // 5 seconds interval

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_SENSOR_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        puts("DHT sensor initialization failed");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Failed to read data from sensor");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        }

        xtimer_periodic_wakeup(&last_wakeup, SLEEP_INTERVAL);
    }

    return 0;
}