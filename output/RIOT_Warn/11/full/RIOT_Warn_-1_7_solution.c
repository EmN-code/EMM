#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "thread.h"
#include "dht_params.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 4)
#define LED_PIN GPIO_PIN(0, 5)
#define TEMP_THRESHOLD 30

static char thread_stack[THREAD_STACKSIZE_MAIN];

void *dht_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;
    dht_t dev;
    dht_init(&dev, &dht_params[0]);

    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Temperature exceeds threshold! LED turned on.\n");
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_thread");
    return 0;
}