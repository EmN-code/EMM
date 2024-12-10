
#include "thread.h"
#include "MQTTClient.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "mma8x5x_regs.h"
#include "periph/i2c.h"
#include "dht_params.h"
#include "event/thread.h"
#include "dht.h"
#include "mma8x5x_params.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static dht_t dht_dev;

void read_dht(void) {
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
        } else {
            puts("Failed to read from DHT sensor");
        }
        ztimer_sleep(ZTIMER_USEC, SLEEP_INTERVAL);
    }
}

int main(void) {
    if (gpio_init(DHT_PIN, GPIO_OUT) < 0) {
        puts("Failed to initialize GPIO");
        return 1;
    }

    dht_params_t params = { .pin = DHT_PIN };
    if (dht_init(&dht_dev, &params) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        read_dht,
        NULL, "dht_reader"
    );

    return 0;
}
