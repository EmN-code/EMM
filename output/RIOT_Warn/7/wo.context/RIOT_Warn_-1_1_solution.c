#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 4)  // specify the PIN you connect DHT11
#define LED_PIN GPIO_PIN(0, 5)  // specify the PIN connected to LED
#define THRESHOLD_TEMPERATURE 30
#define SLEEP_INTERVAL (5U * US_PER_SEC)

dht_t dht_dev;

int main(void) {
    // Initialize DHT11 device
    if (dht_init(&dht_dev, &dht_params[0]) != DHT_OK)
    {
        printf("DHT initialization failed\n");
        return 1;
    }

    // Initialize LED pin
    if (gpio_init(LED_PIN, GPIO_OUT))
    {
        printf("Failed to initialize LED GPIO\n");
        return 1;
    }

    while (1) {
        int16_t temp;
        int16_t hum;

        // Read temperature and humidity
        if (dht_read(&dht_dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);

            // Check temperature threshold
            if (temp > THRESHOLD_TEMPERATURE) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Sleep for defined interval
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }

    return 0;
}