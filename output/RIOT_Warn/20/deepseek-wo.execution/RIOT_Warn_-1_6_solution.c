
#include "mma8x5x.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "dht.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "paho_mqtt.h"
#include "periph/gpio.h"
#include "dht_params.h"
#include "thread.h"

#include <stdio.h>  // Include stdio.h to resolve printf errors

#define DHT_PIN         GPIO_PIN(0, 18)  // Example GPIO pin for DHT11
#define LED_PIN         GPIO_PIN(0, 2)   // Example GPIO pin for LED
#define TEMP_THRESHOLD  30               // Temperature threshold in Celsius

static dht_t dht_dev;

void init_dht(void) {
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11,
    };
    dht_init(&dht_dev, &dht_params);
}

void init_led(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);
}

int main(void) {
    init_dht();
    init_led();

    int16_t temp, hum;

    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == 0) {
            printf("Temperature: %d C, Humidity: %d %%\n", temp, hum);

            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read DHT sensor\n");
        }

        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}
