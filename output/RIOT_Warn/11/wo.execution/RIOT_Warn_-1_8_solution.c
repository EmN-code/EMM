#include "thread.h"
#include "string.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"

#define DHT_PIN GPIO_PIN(0, 5) // Change according to actual setup
#define LED_PIN GPIO_PIN(0, 10) // Change according to actual setup
#define TEMP_THRESHOLD 30 // Temperature threshold in °C

void *thread_func(void *arg) {
    (void)arg;
    int16_t temp, hum;
    dht_t dev;

    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize DHT sensor
    dev.pin = DHT_PIN;

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    thread_create(NULL, thread_func, NULL, 0, "sensor_thread");
    return 0;
}