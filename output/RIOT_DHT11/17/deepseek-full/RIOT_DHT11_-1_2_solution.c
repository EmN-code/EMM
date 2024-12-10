
#include "thread.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "event/thread.h"
#include "dht.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define SAMPLE_INTERVAL (5 * 1000 * 1000UL) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_MAIN];

void* sensor_thread(void* arg) {
    (void)arg;

    dht_t dev;
    dht_params_t params = {
        .pin = DHT11_PIN,
        .type = DHT11
    };

    if (dht_init(&dev, &params) != DHT_OK) {
        puts("DHT11 initialization failed");
        return NULL;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            puts("Failed to read from DHT11 sensor");
        }

        xtimer_usleep(SAMPLE_INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  sensor_thread, NULL, "sensor_thread");

    return 0;
}
