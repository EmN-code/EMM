#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11

void *dht_reading_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    // Initialize DHT sensor
    if (dht_init(&dev, &dht_params[0]) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return NULL;
    }

    while (1) {
        int res = dht_read(&dev, &temp, &hum);
        if (res == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("DHT reading failed\n");
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, dht_reading_thread, NULL, "dht_reading");

    // Keep the main thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}