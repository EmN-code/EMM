
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "event/thread.h"
#include "thread.h"
#include "dht_params.h"
#include <stdio.h>

#define DHT_PIN             GPIO_PIN(0, 18)
#define LED_PIN             GPIO_PIN(0, 2)
#define TEMP_THRESHOLD      30

static void read_dht_and_check_temp(void)
{
    int16_t temp, hum;
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE_DHT11
    };

    if (dht_read(&dht_params, &temp, &hum) == 0) {
        printf("Temperature: %d, Humidity: %d\n", temp, hum);
        if (temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Failed to read DHT sensor\n");
    }
}

int main(void)
{
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        read_dht_and_check_temp();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
