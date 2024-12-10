#include "periph/gpio.h"
#include "periph/dht.h"
#include "xtimer.h"
#include <stdio.h>

#define THRESHOLD_TEMP 30  // Threshold temperature in Celsius
#define LED_PORT GPIO_PIN(PORT_A, 0)  // Define the port and pin for the LED

dht_t dev;

int main(void)
{
    // Initialize the LED as output
    gpio_init(LED_PORT, GPIO_OUT);

    // Initialize DHT sensor (assuming DHT11 is on GPIO_PIN(PORT_A, 1))
    dev.params.pin = GPIO_PIN(PORT_A, 1);
    int init = dht_init(&dev);

    if (init != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }
    else {
        printf("DHT initialized\n");
    }

    while (1) {
        int16_t temperature = 0, humidity = 0;

        // Read data from DHT11 sensor
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        }
        else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature / 10, humidity / 10);

            // Check temperature and control LED
            if (temperature / 10 > THRESHOLD_TEMP) {
                gpio_set(LED_PORT);
                printf("Temperature exceeded threshold! LED turned on.\n");
            }
            else {
                gpio_clear(LED_PORT);
            }
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}