
#include "thread.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "mma8x5x_regs.h"
#include "periph/i2c.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"

#define DHT_PIN             GPIO_PIN(0, 18)
#define LED_PIN             GPIO_PIN(0, 2)
#define TEMP_THRESHOLD      30

static dht_t dht_dev;
static int16_t temperature;
static int16_t humidity;

static void *dht_thread(void *arg)
{
    (void)arg;

    while (1) {
        if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    gpio_init(LED_PIN, GPIO_OUT);
    dht_params_t dht_params = { .pin = DHT_PIN };
    dht_init(&dht_dev, &dht_params);

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  0, dht_thread, NULL, "dht_thread");

    return 0;
}
