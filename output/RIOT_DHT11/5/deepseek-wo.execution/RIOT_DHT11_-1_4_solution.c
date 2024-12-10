
#include "periph/gpio.h"
#include "thread.h"
#include "ztimer.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static dht_t dht_dev;

int main(void)
{
    int16_t temperature, humidity;

    // Initialize the DHT11 sensor
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    dht_init(&dht_dev, &dht_params);

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
            // Print the values (Note: printf is not allowed, so we use a placeholder)
            // printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_USEC, SLEEP_INTERVAL);
    }

    return 0;
}
