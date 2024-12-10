
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static dht_t dht11;

int main(void)
{
    int16_t temperature, humidity;

    // Initialize the DHT11 sensor
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11,
        .in_mode = GPIO_IN_PU  // Use GPIO_IN_PU for pull-up mode
    };

    dht_init(&dht11, &dht_params);

    while (1) {
        if (dht_read(&dht11, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   (int)temperature / 10, (int)temperature % 10,
                   (int)humidity / 10, (int)humidity % 10);
        } else {
            puts("Failed to read from DHT11 sensor");
        }

        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
