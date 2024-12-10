#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define TEMP_THRESHOLD 30 // Temperature threshold in degrees Celsius
#define INTERVAL        (5U * US_PER_SEC) // 5 seconds interval

// Define the GPIO pins for DHT11 and LED
#define DHT_PIN         GPIO_PIN(PORT_A, 0)
#define LED_PIN         GPIO_PIN(PORT_A, 1)

int main(void)
{
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT sensor
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    if (dht_init(&dev, &params) != 0) {
        puts("DHT initialization failed");
        return 1;
    }

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) != 0) {
            puts("Failed to read DHT sensor");
        } else {
            printf("Temperature: %d Humidity: %d\n", temp, hum);
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN); // Turn the LED on
            } else {
                gpio_clear(LED_PIN); // Turn the LED off
            }
        }

        // Wait for the next measurement
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}