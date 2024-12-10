#include "xtimer.h"
#include "periph/gpio.h"
#include "dht/dht.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(PORT_B, 22) // Example pin, ensure to use the correct port and pin
#define THRESHOLD_TEMPERATURE 30

int main(void) {
    gpio_init(GPIO_PIN(PORT_C, 18), GPIO_OUT);

    dht_t dev;
    dht_params_t params;

    // Initialize the DHT sensor
    params.pin = DHT_PIN;
    params.type = DHT11;
    dht_init(&dev, &params);

    while (1) {
        int16_t temp, hum;

        // Read from the DHT sensor
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read DHT sensor\n");
        }
        else {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            if (temp >= THRESHOLD_TEMPERATURE * 10) {
                gpio_set(GPIO_PIN(PORT_C, 18)); // Turn on LED
            }
            else {
                gpio_clear(GPIO_PIN(PORT_C, 18)); // Turn off LED
            }
        }

        xtimer_sleep(5); // Wait for 5 seconds
    }

    return 0;
}