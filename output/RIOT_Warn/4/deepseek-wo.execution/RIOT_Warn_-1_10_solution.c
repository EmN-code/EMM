
#include "periph/gpio.h"
#include "ztimer.h"
#include "fmt.h"

#define DHT_PIN            GPIO_PIN(0, 18)  // Example GPIO pin for DHT11
#define LED_PIN            GPIO_PIN(0, 21)  // Example GPIO pin for LED
#define TEMP_THRESHOLD     25  // Example temperature threshold in Celsius

int main(void) {
    int16_t temp, hum;
    gpio_t dht_gpio = DHT_PIN;
    gpio_t led_gpio = LED_PIN;

    // Initialize GPIO pins
    gpio_init(dht_gpio, GPIO_IN);
    gpio_init(led_gpio, GPIO_OUT);

    while (1) {
        // Read temperature and humidity from DHT11
        if (dht_read(&dht_gpio, &temp, &hum) == 0) {
            // Check if temperature exceeds the threshold
            if (temp > TEMP_THRESHOLD) {
                gpio_set(led_gpio);  // Turn on the LED
            } else {
                gpio_clear(led_gpio);  // Turn off the LED
            }
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
