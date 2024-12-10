#include <xtimer.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0) // Assumes the DHT11 is connected to PORT_A, PIN 0
#define LED_PIN GPIO_PIN(PORT_B, 1) // Assumes the LED is connected to PORT_B, PIN 1
#define TEMPERATURE_THRESHOLD 30    // Temperature threshold in Celsius

int main(void) {
    dht_t dev;
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    if (dht_init(&dev, DHT_TYPE_DHT11) != 0) {
        printf("Error: Could not initialize DHT11 sensor\n");
        return 1;
    }

    while (1) {
        int16_t temperature, humidity;
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Error: Could not read data from DHT11 sensor\n");
        } else {
            printf("Temperature: %d °C, Humidity: %d %%RH\n", temperature, humidity);
            if (temperature > TEMPERATURE_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        }
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}