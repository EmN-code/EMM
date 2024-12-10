
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_TYPE            DHT11
#define SLEEP_INTERVAL      (5 * 1000000U)  // 5 seconds in microseconds

static dht_t dht_dev;

int main(void)
{
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE
    };

    if (dht_init(&dht_dev, &params) == DHT_OK) {
        int16_t temperature, humidity;

        while (1) {
            if (dht_read(&dht_dev, &temperature, &humidity) == DHT_OK) {
                // Assuming you have a way to print or log the values
                // Since printf is not allowed, you can use a custom logging function
                // or store the values in a buffer for later processing.
                // For example:
                // log_temperature_humidity(temperature, humidity);
            }

            xtimer_usleep(SLEEP_INTERVAL);
        }
    }

    return 0;
}
