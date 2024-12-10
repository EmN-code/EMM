#include "thread.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "nvs.h"
#include "thread_flags.h"
#include "mma8x5x_regs.h"
#include "nvs_flash.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "mma8x5x_params.h"

#define DHT_PIN 2

void *dht_thread(void *arg) {
    dht_t dev;
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;

    int16_t temp, hum;
    dht_init(&dev, &params);

    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
        } else {
            printf("Error reading DHT11\n");
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, dht_thread, NULL, "dht");
    return 0;
}