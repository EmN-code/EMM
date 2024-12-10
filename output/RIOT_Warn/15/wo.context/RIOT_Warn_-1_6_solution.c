#include <stdio.h>
#include <stdbool.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_B, 0)
#define THRESHOLD_TEMP 30
#define DHT_TYPE DHT11
#define MQTT_BROKER_IP "192.168.1.10"
#define MQTT_BROKER_PORT 1883

void check_temperature(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize the GPIO pin for DHT sensor
    dht_init(&dev, DHT_TYPE, DHT_PIN);

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
            if (temp > THRESHOLD_TEMP) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read DHT sensor\n");
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    // Initialize LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // For network connection setup simulation
    int rc = 0;
    while (1) {
        rc = NetworkConnect(MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Network connected successfully\n");
            break;
        }
    }

    check_temperature();
    return 0;
}