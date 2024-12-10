#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 4)  // assuming the sensor is connected to pin 4
#define LED_PIN GPIO_PIN(0, 5)  // assuming the LED is connected to pin 5
#define TEMP_THRESHOLD 30       // temperature threshold in degrees Celsius

static dht_t dev;

int main(void) {
    // Initialize the DHT sensor
    int res = dht_init(&dev, DHT_PIN, DHT11);
    if (res != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return 1;
    }

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        int16_t temp, hum;
        res = dht_measure(&dev, &temp, &hum);

        if (res == DHT_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);

            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: Temperature exceeds %d°C!\n", TEMP_THRESHOLD);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        xtimer_sleep(5);
    }

    return 0;
}