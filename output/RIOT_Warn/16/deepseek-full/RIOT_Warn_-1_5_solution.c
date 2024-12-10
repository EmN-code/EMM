
#include "thread.h"
#include "paho_mqtt.h"
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

#define DHT_PIN             GPIO_PIN(0, 18)
#define LED_PIN             GPIO_PIN(0, 21)
#define TEMP_THRESHOLD      25
#define SLEEP_DURATION      5000000

static void *dht_thread(void *arg) {
    (void)arg;

    dht_t dht;
    int16_t temp, hum;

    while (1) {
        if (dht_read(&dht, &temp, &hum) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read DHT sensor\n");
        }
        xtimer_usleep(SLEEP_DURATION);
    }

    return NULL;
}

int main(void) {
    gpio_init(DHT_PIN, GPIO_OUT);
    gpio_init(LED_PIN, GPIO_OUT);

    thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        dht_thread, NULL, "dht_thread"
    );

    return 0;
}
