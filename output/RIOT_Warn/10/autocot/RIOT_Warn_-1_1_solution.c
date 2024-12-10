#include <stdio.h>
#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_GPIO_PIN    GPIO_PIN(0, 15)  // Example GPIO pin for DHT11
#define LED_GPIO_PIN    GPIO_PIN(0, 2)   // Example GPIO pin for LED
#define THRESHOLD_TEMP  30               // Temperature threshold in Celsius

int main(void)
{
    /* Initialize DHT11 */
    dht_t dev;
    dht_params_t params;
    params.pin = DHT_GPIO_PIN;
    params.type = DHT11;
    if (dht_init(&dev, &params) != 0) {
        printf("DHT11 initialization failed\n");
        return 1;
    }

    /* Initialize the LED pin */
    gpio_init(LED_GPIO_PIN, GPIO_OUT);

    /* Variables to store temperature and humidity */
    int16_t temperature, humidity;

    while (1) {
        /* Read values from DHT11 */
        if (dht_read(&dev, &temperature, &humidity) == DHT_ERR_NONE) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
            
            /* Check temperature threshold */
            if (temperature > THRESHOLD_TEMP) {
                gpio_set(LED_GPIO_PIN);
                printf("Temperature threshold exceeded! LED turned ON.\n");
            } else {
                gpio_clear(LED_GPIO_PIN);
                printf("Temperature is within limit. LED turned OFF.\n");
            }
        } else {
            printf("Failed to read from DHT11\n");
        }

        /* Wait 5 seconds */
        xtimer_sleep(5);
    }

    return 0;
}