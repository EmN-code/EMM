
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "dht.h"
#include "MQTTClient.h"
#include "periph/i2c.h"
#include "dht_params.h"
#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "mma8x5x_regs.h"
#include "thread.h"
#include "periph/gpio.h"
#include "event/thread.h"

#define TEMP_THRESHOLD 30
#define LED_PIN GPIO_PIN(0, 13)
#define DHT_PIN GPIO_PIN(0, 12)

void read_dht_and_check_temp(void) {
    int16_t temp, hum;
    dht_read(&(dht_t){.pin = DHT_PIN}, &temp, &hum);

    if (temp > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        read_dht_and_check_temp();
        xtimer_sleep(5);
    }

    return 0;
}
