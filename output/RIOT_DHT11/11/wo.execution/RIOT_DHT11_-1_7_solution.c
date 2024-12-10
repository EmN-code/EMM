#include "paho_mqtt.h"
#include "mqttclient.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "event/thread.h"
#include "thread.h"

#define DHT_PIN 2

dht_t dev;

void *read_dht(void *arg) {
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT
");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%
", temp / 10, temp % 10, hum / 10, hum % 10);
        }
        thread_sleep(5);
    }
    return NULL;
}

int main(void) {
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT_TYPE_DHT11;

    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT
");
        return 1;
    }

    thread_create(read_dht, NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, "dht_reader");

    return 0;
}