#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT11_PIN GPIO_PIN(0, 5)
#define WARNING_LED_PIN GPIO_PIN(0, 2)
#define TEMP_THRESHOLD 30  // Temperature threshold in Celsius
#define READ_INTERVAL_SEC (5U)

int main(void) {
    // Initialize GPIO pins
    gpio_init(WARNING_LED_PIN, GPIO_OUT);
    gpio_clear(WARNING_LED_PIN);

    dht_params_t DHT11_params;
    DHT11_params.pin = DHT11_PIN;

    dht_t dev;
    if (dht_init(&dev, &DHT11_params) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return -1;
    }

    int16_t temperature, humidity;

    while(1) {
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Failed to read data from sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                    temperature / 10, temperature % 10,
                    humidity / 10, humidity % 10);

            // Check if temperature exceeds threshold
            if ((temperature / 10) > TEMP_THRESHOLD) {
                gpio_set(WARNING_LED_PIN);
            } else {
                gpio_clear(WARNING_LED_PIN);
            }
        }
        
        // Sleep for defined interval
        xtimer_sleep(READ_INTERVAL_SEC);
    }

    return 0;
}