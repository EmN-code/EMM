
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static dht_t dht11;

void read_dht11(void) {
    int16_t temp, hum;
    if (dht_read(&dht11, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read from DHT11 sensor");
    }
}

int main(void) {
    // Initialize the DHT11 sensor
    if (dht_init(&dht11, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        puts("DHT11 initialization failed");
        return 1;
    }

    // Set up a timer to read the DHT11 sensor every 5 seconds
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        read_dht11();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    return 0;
}
