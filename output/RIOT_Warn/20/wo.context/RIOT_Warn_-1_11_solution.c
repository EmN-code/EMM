#include <xtimer.h>
#include <dht.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 10) // Define the LED pin
#define DHT_PIN GPIO_PIN(PORT_A, 0)  // Define the DHT data pin
#define TEMP_THRESHOLD 30            // Temperature threshold in degree Celsius

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_t dev;
    dht_init(&dev, DHT_PIN, DHT11);
    
    int16_t temp, hum;
    
    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            // Handle read error
            gpio_clear(LED_PIN);
        }
        xtimer_sleep(5);
    }
    return 0;
}