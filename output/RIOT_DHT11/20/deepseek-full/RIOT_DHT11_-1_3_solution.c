
#include "thread.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "mma8x5x_regs.h"
#include "periph/i2c.h"
#include "dht_params.h"
#include "event/thread.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static void *dht_thread(void *arg)
{
    (void)arg;
    int16_t temperature, humidity;

    while (1) {
        if (dht_read(NULL, &temperature, &humidity) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
        } else {
            puts("Failed to read from DHT sensor");
        }

        ztimer_sleep(ZTIMER_USEC, SLEEP_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    if (gpio_init(DHT_PIN, GPIO_OUT) < 0) {
        puts("Failed to initialize GPIO");
        return 1;
    }

    thread_t *dht_thread_handle;
    char dht_stack[THREAD_STACKSIZE_DEFAULT];

    dht_thread_handle = thread_create(dht_stack, sizeof(dht_stack),
                                      THREAD_PRIORITY_MAIN - 1,
                                      THREAD_CREATE_STACKTEST,
                                      dht_thread, NULL, "dht_thread");

    if (dht_thread_handle == NULL) {
        puts("Failed to create DHT thread");
        return 1;
    }

    return 0;
}
