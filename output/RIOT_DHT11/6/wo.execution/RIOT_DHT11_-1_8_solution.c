#include <thread.h>
#include <mma8x5x.h>
#include <MQTTClient.h>
#include <dht_params.h>
#include <dht.h>
#include <event/thread.h>
#include <paho_mqtt.h>
#include <periph/rtc.h>
#include <nvs.h>
#include <thread_flags.h>
#include <mma8x5x_regs.h>
#include <nvs_flash.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <mma8x5x_params.h>

#define DHT_PIN 2
#define THREAD_PRIORITY_MAIN 3
#define DHT_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

char stack[DHT_STACKSIZE];

dht_params_t dht_params = {
    .pin = DHT_PIN,
    .type = DHT_TYPE_DHT11
};

dht_t dev;

void *dht11_reader(void *arg) {
    (void)arg;
    int16_t temp, hum;
    while (1) {
        int res = dht_read(&dev, &temp, &hum);
        if (res == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    puts("DHT11 Sensor Reader started");
    if (dht_init(&dev, &dht_params) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                  dht11_reader, NULL, "dht11_reader");
    return 0;
}