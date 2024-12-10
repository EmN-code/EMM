#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <board.h>
#include <periph/gpio.h>

#define DHT11_PIN GPIO_PIN(PORT_A, 0) // Define the pin for the DHT11 sensor
#define LED_PIN GPIO_PIN(PORT_B, 1)   // Define the pin for the LED
#define TEMP_THRESHOLD 30             // Temperature threshold in degree Celsius

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    dht_init(&dev, DHT11_PIN);
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Sensor read failed\n");
        } else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);

            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: Temperature exceeded threshold!\n");
            } else {
                gpio_clear(LED_PIN);
            }
        }

        xtimer_sleep(5);
    }

    return 0;
}