
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "mma8x5x_regs.h"
#include "dht.h"
#include "event/thread.h"
#include "dht_params.h"
#include "thread.h"

#define DHT_PIN GPIO_PIN(0, 2)

static dht_t dht_dev;

int main(void)
{
    int16_t temperature, humidity;

    // Initialize the DHT11 sensor
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11,
    };
    dht_init(&dht_dev, &dht_params);

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
            // Print the values
            puts("Temperature: ");
            puts(temperature);
            puts(" °C, Humidity: ");
            puts(humidity);
            puts(" %\n");
        } else {
            puts("Failed to read from DHT sensor\n");
        }

        // Wait for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
