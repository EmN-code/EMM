
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5 * 1000 * 1000U)  // 5 seconds in microseconds

static void read_dht(void) {
    dht_data_t data;
    int res = dht_read(DHT_TYPE_DHT11, DHT_PIN, &data);
    if (res == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               data.temperature / 10, data.temperature % 10,
               data.humidity / 10, data.humidity % 10);
    } else {
        printf("Error reading DHT11 sensor\n");
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Initialize the DHT11 sensor
    if (dht_init(DHT_TYPE_DHT11, DHT_PIN) == DHT_OK) {
        printf("DHT11 sensor initialized\n");
    } else {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    while (1) {
        read_dht();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    return 0;
}
