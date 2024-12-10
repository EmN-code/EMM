#include "mma8x5x_params.h"
#include "dht_params.h"
#include "thread.h"
#include "dht.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5U * 1000000U)

void *dht_reader(void *arg) {
    (void)arg;

    dht_t dev;
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT_TYPE_DHT11;

    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize DHT11\n");
        return NULL;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Failed to read DHT11 sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        }
        char stack[THREAD_STACKSIZE_MAIN];
        xtimer_usleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, dht_reader, NULL, "dht_reader");
    while (1) {
        thread_sleep();
    }
    return 0;
}