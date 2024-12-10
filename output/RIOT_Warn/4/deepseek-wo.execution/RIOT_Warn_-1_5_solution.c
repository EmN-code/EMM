
#include "mma8x5x_params.h"
#include "dht.h"
#include "MQTTClient.h"
#include "periph/i2c.h"
#include "dht_params.h"
#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "thread.h"
#include "periph/gpio.h"
#include "event/thread.h"

#define THRESHOLD_TEMP 30
#define LED_PIN GPIO_PIN(0, 13)
#define DHT_PIN GPIO_PIN(0, 12)

static void read_dht11(void *arg) {
    (void)arg;
    while (1) {
        int16_t temp, hum;
        if (dht_read(&dht_params[0], &temp, &hum) == 0) {
            if (temp > THRESHOLD_TEMP) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_init(&dht_params[0]);

    void (*task_func)(void *) = (void (*)(void *))read_dht11;
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, task_func, NULL, "dht_reader");

    return 0;
}
