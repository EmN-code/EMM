#include <xtimer.h>
#include <timex.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT11_PIN GPIO_PIN(0, 17)   // Define the GPIO pin connected to the DHT11 data pin
#define LED_PIN GPIO_PIN(0, 2)      // Define the GPIO pin connected to the LED
#define TEMP_THRESHOLD 25           // Temperature threshold for LED warning

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_init(DHT11_PIN, GPIO_IN);
    dht_t dev;
    int16_t temperature, humidity;
    
    if (dht_init(&dev, DHT11_PIN, DHT11) != DHT_OK) {
        printf("Error initializing DHT11 sensor\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &humidity, &temperature) == DHT_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature / 10, humidity / 10);
            if ((temperature / 10) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Error reading DHT11 sensor\n");
        }

        xtimer_sleep(5);  // wait 5 seconds
    }
    return 0;
}