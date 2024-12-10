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
#include <stdio.h>

#define TEMP_THRESHOLD 30
#define DHT_PIN GPIO_PIN(PORT_B, 5)
#define LED_PIN GPIO_PIN(PORT_A, 10)

void configure_gpio(void);
void read_sensor(void);
void handle_warning(int16_t temp);

int main(void) {
    configure_gpio();

    while (1) {
        read_sensor();
        ztimer_sleep(ztimer_now() + 5000*US_PER_MS);
    }

    return 0;
}

void configure_gpio(void) {
    // Initialize GPIO
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);
}

void read_sensor(void) {
    int16_t temperature, humidity;
    dht_t sensor;
    sensor.pin = DHT_PIN;
    sensor.type = DHT11;

    if (dht_read(&sensor, &temperature, &humidity) != ESP_OK) {
        printf("Error reading DHT sensor\n");
    } else {
        printf("Temperature: %d, Humidity: %d\n", temperature / 10, humidity / 10);
        handle_warning(temperature);
    }
}

void handle_warning(int16_t temp) {
    if ((temp / 10) > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}