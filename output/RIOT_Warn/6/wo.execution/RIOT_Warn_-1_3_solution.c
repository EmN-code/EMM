#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "thread.h"
#include "event/thread.h"
#include "dht.h"
#include "MQTTClient.h"

#define DHT_PIN GPIO_PIN(PORT_A, 5)
#define LED_PIN GPIO_PIN(PORT_B, 0)
#define TEMP_THRESHOLD 30

static char stack[THREAD_STACKSIZE_DEFAULT];

void *temperature_monitor(void *arg) {
    (void)arg;
    int16_t temp, hum;
    while (1) {
        if (dht_read(DHT_PIN, &temp, &hum) == 0 && temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_init(DHT_PIN);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  temperature_monitor, NULL, "temperature_monitor");
    while (1) {
        // Main thread can do other tasks, such as managing MQTT messages
    }
    return 0;
}