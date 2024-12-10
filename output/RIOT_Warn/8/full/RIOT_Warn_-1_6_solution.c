#include <string.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <thread.h>
#include <dht.h>
#include <mma8x5x.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <mma8x5x_regs.h>
#include <event/thread.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <periph/i2c.h>

#define DHT_PIN GPIO_PIN(0, 5)
#define LED_PIN GPIO_PIN(0, 18)
#define TEMP_THRESHOLD 30

static char stack[THREAD_STACKSIZE_MAIN];

void *dht_read_thread(void *arg) {
    dht_t dev;
    int16_t temperature, humidity;
    (void) arg;

    if (dht_init(&dev, &dht_params[0]) != 0) {
        printf("Failed to initialize DHT11\n");
        return NULL;
    }

    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("DHT read failed\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
            if (temperature > TEMP_THRESHOLD * 10) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        ztimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, dht_read_thread, NULL, "dht_read");
    return 0;
}