
#include <stdio.h>
#include "board.h"
#include "periph/gpio.h"
#include "xtimer.h"
#include "dht11_sensor.h"

#define DHT11_PIN GPIO_PIN(PORT_A, 0)
#define RGB_LED_PIN GPIO_PIN(PORT_A, 1)

int main(void)
{
    // Initialize the DHT11 sensor
    if (dht11_init(DHT11_PIN) != DHT11_OK) {
        printf("DHT11 initialization failed\n");
        return 1;
    }

    // Initialize the RGB LED
    gpio_init(RGB_LED_PIN, GPIO_OUT);

    while (1) {
        // Read the DHT11 sensor
        int16_t temp, hum;
        if (dht11_read(&temp, &hum) == DHT11_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Turn on the RGB LED
        gpio_set(RGB_LED_PIN);

        // Wait for 5 seconds
        xtimer_sleep(5);

        // Turn off the RGB LED
        gpio_clear(RGB_LED_PIN);
    }

    return 0;
}
