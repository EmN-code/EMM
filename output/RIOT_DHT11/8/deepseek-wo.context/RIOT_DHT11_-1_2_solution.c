
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN         GPIO_PIN(0, 2)  // GPIO 2 on ESP32
#define DHT_TYPE        DHT11
#define SLEEP_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static dht_t dht_dev;

static void read_dht(void)
{
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read from DHT sensor");
    }
}

int main(void)
{
    // Initialize the DHT11 sensor
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE,
        .in_mode = GPIO_IN_PU  // Use GPIO_IN_PU for pull-up mode
    };

    if (dht_init(&dht_dev, &dht_params) == DHT_OK) {
        puts("DHT11 sensor initialized");
    } else {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    while (1) {
        read_dht();
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
