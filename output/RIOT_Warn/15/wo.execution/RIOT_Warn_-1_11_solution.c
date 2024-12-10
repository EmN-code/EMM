#include "ztimer.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 10)
#define LED_PIN GPIO_PIN(PORT_A, 5)
#define TEMP_THRESHOLD 30

int main(void) {
    dht_t dev;
    dht_params_t params;
    params.pin = DHT_PIN;
    dht_init(&dev, &params);

    // configure the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        int16_t temp, hum;
        dht_read(&dev, &temp, &hum);

        printf("Temperature: %d.%dC, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }

        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return 0;
}