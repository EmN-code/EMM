#include <stdio.h>
#include <dht.h>
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0) // Define the GPIO pin where the DHT11 is connected
#define LED_PIN GPIO_PIN(PORT_A, 1) // Define the GPIO pin for the LED
#define TEMP_THRESHOLD 30           // Define the temperature threshold (e.g., 30°C)

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    dev.params.pin = DHT_PIN; // Set the DHT pin
    dev.params.type = DHT_TYPE_DHT11; // Use DHT11 type

    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        if (dht_measure(&dev, &temp, &hum) != 0) {
            printf("Error reading from DHT sensor\n");
        }
        else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            }
            else {
                gpio_clear(LED_PIN);
            }
        }

        xtimer_sleep(5); // Wait for 5 seconds before next measurement
    }
    return 0;
}
