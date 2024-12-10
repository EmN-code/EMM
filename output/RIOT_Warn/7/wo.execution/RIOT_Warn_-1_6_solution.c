#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "thread.h"

#define LED_PIN GPIO_PIN(0, 2)
#define TEMP_THRESHOLD 30

void *read_dht(void *arg) {
    (void) arg;
    dht_t dev;
    int16_t temperature, humidity;
    
    while (1) {
        dht_read(&dev, &temperature, &humidity);
        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    thread_create(read_dht, NULL, THREAD_PRIORITY_MAIN - 1, 0);
    pause();  // Keep the main thread active while the read_dht thread runs
    return 0;
}