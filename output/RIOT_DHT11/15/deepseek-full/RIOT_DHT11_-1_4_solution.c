
#include "thread.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "periph/i2c.h"
#include "dht_params.h"
#include "event/thread.h"
#include "dht.h"
#include "mma8x5x_params.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define SLEEP_DURATION      (5 * 1000000U)  // 5 seconds in microseconds

static dht_t dht_dev;

void read_dht_sensor(void)
{
    int16_t temperature, humidity;
    while (1) {
        if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
        } else {
            puts("Failed to read from DHT sensor");
        }
        xtimer_usleep(SLEEP_DURATION);
    }
}

int main(void)
{
    if (gpio_init(DHT_PIN, GPIO_OUT) < 0) {
        puts("Failed to initialize GPIO");
        return 1;
    }

    dht_params_t params = { .pin = DHT_PIN };
    if (dht_init(&dht_dev, &params) != 0) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    read_dht_sensor();

    return 0;
}
