
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "mma8x5x_regs.h"
#include "dht.h"
#include "event/thread.h"
#include "dht_params.h"
#include "thread.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_TYPE            DHT11

static dht_t dht_dev;

int main(void)
{
    int16_t temperature, humidity;

    /* Initialize the DHT device */
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE,
    };

    if (dht_init(&dht_dev, &dht_params) != DHT_OK) {
        puts("DHT11 initialization failed");
        return 1;
    }

    while (1) {
        /* Read temperature and humidity */
        if (dht_read(&dht_dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            puts("Failed to read data from DHT11");
        }

        /* Wait for 5 seconds */
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
