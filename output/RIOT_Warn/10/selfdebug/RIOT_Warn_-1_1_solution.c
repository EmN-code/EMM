#include "xtimer.h"
#include "timex.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN    GPIO_PIN(0, 4) // Pin where the DHT11 is connected
#define LED_PIN    GPIO_PIN(0, 5) // Pin where the LED is connected
#define TEMP_THRESH 30            // Temperature threshold
#define DELAY      (5U * US_PER_SEC) // 5 seconds delay

int main(void)
{
    dht_t dev;
    int16_t temp, hum;

    // Initialize DHT sensor
    if (dht_init(&dev, &DHT_PIN) != 0) {
        puts("Error initializing DHT sensor");
        return 1;
    }

    // Initialize LED pin
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        puts("Error initializing LED pin");
        return 1;
    }

    while (1) {
        // Read sensor values
        if (dht_read(&dev, &temp, &hum) != 0) {
            puts("Error reading from DHT sensor");
        } else {
            // Print temperature and humidity
            printf("Temperature: %d, Humidity: %d\n", temp, hum);

            // Check temperature threshold
            if (temp > TEMP_THRESH) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Wait 5 seconds
        xtimer_sleep(DELAY);
    }

    return 0;
}