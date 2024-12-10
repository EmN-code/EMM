#include "xtimer.h"
#include "gpio.h"
#include "dht.h"
#include "thread.h"
#include "stdint.h"
#include "stdio.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0) // Define your DHT pin
#define LED_PIN GPIO_PIN(PORT_B, 1) // Define your LED pin
#define TEMP_THRESHOLD 30           // temperature threshold

static dht_t dev;

void read_sensor(void) {
    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Error reading sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Temperature exceeded threshold! LED turned on.\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature is below threshold. LED turned off.\n");
            }
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    // initialize DHT sensor
    if (dht_init(&dev, DHT_PIN) != 0) {
        printf("DHT initialization failed\n");
        return 1;
    }
    printf("DHT initialized successfully\n");

    // configure LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // start reading the sensor in a loop
    read_sensor();

    return 0;
}