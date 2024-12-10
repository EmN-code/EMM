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

#define DHT_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_B, 0)
#define TEMPERATURE_THRESHOLD 30

static dht_t dev;

static void *dht_sensor_thread(void *arg) {
    (void)arg;
    int16_t temperature, humidity;

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %0.1f°C, Humidity: %0.1f%%\n", temperature / 10.0, humidity / 10.0);
            if (temperature / 10 > TEMPERATURE_THRESHOLD) {
                gpio_set(LED_PIN);
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
    if (dht_init(&dev, &dht_params[DHT_TYPE_DHT11], DHT_PIN) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    char thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_sensor_thread, NULL, "dht_thread");

    while (1) {
        pause();
    }

    return 0;
}
