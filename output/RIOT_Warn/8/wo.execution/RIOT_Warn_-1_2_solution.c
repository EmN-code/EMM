#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "event/thread.h"
#include "dht.h"
#include "xtimer.h"

#define LED_PIN GPIO_PIN(PORT_B, 0)
#define DHT_PIN GPIO_PIN(PORT_A, 0)
#define TEMP_THRESHOLD 30 // Temperature threshold for LED warning

int main(void) {
    // Initialize GPIO pin for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize DHT sensor
    dht_t dev;
    dev.params.pin = DHT_PIN;

    int16_t temp, hum;
    
    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            // Convert temperature to Celsius
            float temp_c = temp / 10.0;

            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN); // Turn on LED if temperature exceeds threshold
            } else {
                gpio_clear(LED_PIN); // Turn off LED
            }
        }
        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return 0;
}