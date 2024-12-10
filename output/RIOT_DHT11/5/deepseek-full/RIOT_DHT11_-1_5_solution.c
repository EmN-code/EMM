
#include "periph/gpio.h"
#include "xtimer.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_TYPE            DHT11
#define SLEEP_INTERVAL      (5 * 1000000U)  // 5 seconds in microseconds

static dht_t dht_dev;

int main(void)
{
    int16_t temperature, humidity;
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE
    };

    if (dht_init(&dht_dev, &dht_params) == DHT_OK) {
        while (1) {
            if (dht_read(&dht_dev, &temperature, &humidity) == DHT_OK) {
                printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                       temperature / 10, temperature % 10,
                       humidity / 10, humidity % 10);
            } else {
                puts("Failed to read from DHT sensor");
            }
            xtimer_usleep(SLEEP_INTERVAL);
        }
    } else {
        puts("Initialization of DHT sensor failed");
    }

    return 0;
}
