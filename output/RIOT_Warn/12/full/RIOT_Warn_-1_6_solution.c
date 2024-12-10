#include "mma8x5x_params.h"
#include "dht_params.h"
#include "thread.h"
#include "dht.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"

#define TEMP_THRESHOLD 30  // Temperature threshold
#define LED_PIN 2  // LED pin

dht_t dht_dev;

gpio_t led_pin = LED_PIN;

void read_dht_and_control_led(void) {
    int16_t temperature, humidity;
    if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
        printf("Temperature: %d, Humidity: %d\n", temperature / 10, humidity / 10);
        if ((temperature / 10) > TEMP_THRESHOLD) {
            gpio_set(led_pin);
        } else {
            gpio_clear(led_pin);
        }
    } else {
        printf("Failed to read from DHT sensor\n");
    }
}

int main(void) {
    printf("Initializing DHT sensor...\n");
    dht_params_t params = DHT_PARAMS;
    dht_init(&dht_dev, &params);

    printf("Configuring LED pin...\n");
    gpio_init(led_pin, GPIO_OUT);

    while (1) {
        read_dht_and_control_led();
        xtimer_sleep(5);
    }

    return 0;
}