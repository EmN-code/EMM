
#include "thread.h"
#include "dht_params.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "mma8x5x_regs.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "dht.h"
#include "mma8x5x_params.h"
#include "event/thread.h"

#define LED_PIN             GPIO_PIN(0, 13)
#define DHT_PIN             GPIO_PIN(0, 14)
#define TEMP_THRESHOLD      25
#define SLEEP_DURATION      5000000  // 5 seconds in microseconds

static void read_dht_and_check_temp(void)
{
    int16_t temp, hum;
    dht_t dht;

    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11
    };

    dht_init(&dht, &dht_params);

    if (dht_read(&dht, &temp, &hum) == 0) {
        if (temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    }
}

int main(void)
{
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        read_dht_and_check_temp();
        ztimer_sleep(ZTIMER_USEC, SLEEP_DURATION);
    }

    return 0;
}
