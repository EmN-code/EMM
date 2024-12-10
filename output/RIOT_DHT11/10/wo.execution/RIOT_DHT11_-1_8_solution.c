#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "timex.h"
#include "thread.h"
#include "msg.h"
#include "periph/gpio.h"
#include "dht.h"
#include "xtimer.h"

#define DHT_PIN GPIO_PIN(0, 2)  // Correctly define the GPIO pin for DHT11, GPIO2 on port 0
#define DELAY 5  // Delay in seconds

int main(void) {
    printf("Starting DHT11 sensor reading\n");

    dht_t dev;
    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    while (1) {
        int16_t temperature, humidity;

        // Read values from the sensor
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Failed to read from DHT11 sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        }

        xtimer_sleep(DELAY);
    }

    return 0;
}