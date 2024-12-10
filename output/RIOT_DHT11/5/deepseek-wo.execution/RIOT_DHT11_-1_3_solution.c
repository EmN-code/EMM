
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "dht_params.h"
#include "thread.h"
#include "dht.h"
#include "event/thread.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_TYPE            DHT11
#define SLEEP_INTERVAL      (5 * 1000000U)  // 5 seconds in microseconds

static dht_t dht_dev;

static void *dht_read_thread(void *arg)
{
    (void)arg;
    int16_t temp, hum;

    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            puts("Failed to read from DHT sensor");
        }

        xtimer_usleep(SLEEP_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE,
    };

    if (dht_init(&dht_dev, &dht_params) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    thread_create(
        thread_stack(dht_read_stack),
        sizeof(dht_read_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        dht_read_thread,
        NULL,
        "dht_read"
    );

    return 0;
}
