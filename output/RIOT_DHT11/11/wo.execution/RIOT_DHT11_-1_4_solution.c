#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_params.h"
#include "paho_mqtt.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"
#include "thread.h"

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define GPIO_PIN_DHT            (2)
#define MEASURE_INTERVAL_MS     (5000)

void *dht_reader_thread(void *arg) {
    (void)arg;
    int16_t temperature, humidity;
    dht_t dev;
    dht_params_t dht_params;
    dht_params.pin = GPIO_PIN(GPIO_PIN_DHT);
    dht_params.type = DHT11;

    if (dht_init(&dev, &dht_params) == DHT_OK) {
        while (1) {
            if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
                printf("Temperature: %0.1f°C, Humidity: %0.1f%%\n", temperature / 10.0, humidity / 10.0);
            } else {
                printf("Failed to read data from DHT11 sensor\n");
            }
            thread_sleep(MEASURE_INTERVAL_MS);
        }
    } else {
        printf("Failed to initialize DHT11 sensor\n");
    }

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, dht_reader_thread, NULL, "dht_reader");
    return 0;
}