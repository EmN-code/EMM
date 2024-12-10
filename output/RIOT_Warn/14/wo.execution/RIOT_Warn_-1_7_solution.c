#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "dht_params.h"
#include "dht.h"
#include "periph/gpio.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "thread.h"

#define GPIO_PIN_LED GPIO_NUM_2
#define TEMP_THRESHOLD 30

void control_temperature_warning(gpio_t gpio_led, int16_t temperature);

int main(void) {
    gpio_init(GPIO_PIN_LED, GPIO_OUT);
    dht_t dev;
    int16_t temp, hum;

    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            control_temperature_warning(GPIO_PIN_LED, temp);
        }
        xtimer_sleep(5);
    }
    return 0;
}

void control_temperature_warning(gpio_t gpio_led, int16_t temperature) {
    if (temperature > TEMP_THRESHOLD) {
        gpio_set(gpio_led);
    } else {
        gpio_clear(gpio_led);
    }
}