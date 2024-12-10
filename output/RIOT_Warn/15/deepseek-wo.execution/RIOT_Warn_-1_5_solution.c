
#include "periph/gpio.h"
#include "ztimer.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"

#define DHT_PIN            GPIO_PIN(0, 18)
#define LED_PIN            GPIO_PIN(0, 21)
#define TEMP_THRESHOLD     30

static mma8x5x_t dev;

void init_sensors(void) {
    mma8x5x_init(&dev, &mma8x5x_params[0]);
}

int read_temperature(int16_t *temp) {
    int16_t dummy_hum;
    return mma8x5x_read(&dev, temp, &dummy_hum);
}

void check_temperature(int16_t temp) {
    if (temp > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    init_sensors();

    while (1) {
        int16_t temp;
        if (read_temperature(&temp) == 0) {
            check_temperature(temp);
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
