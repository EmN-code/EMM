#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "thread.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"

#define DHT_GPIO_PIN 2

static dht_t dev;

void *dht_read_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Error reading DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return NULL;
}

int main(void) {
    dht_params_t params;
    params.pin = DHT_GPIO_PIN;
    params.type = DHT11;

    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, dht_read_thread, NULL, "dht_read_thread");

    return 0;
}