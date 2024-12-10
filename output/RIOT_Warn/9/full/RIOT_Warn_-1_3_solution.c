#include "thread.h"
#include "periph/gpio.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "periph/i2c.h"

#define TEMP_THRESHOLD 30  // Temperature threshold
#define GPIO_PIN_LED GPIO_PIN(0, 4)  // Example GPIO pin for LED
#define DHT_PIN GPIO_PIN(0, 5)  // Example GPIO pin for DHT sensor

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    int res;

    // Initialize GPIO pin for LED
    gpio_init(GPIO_PIN_LED, GPIO_OUT);
    gpio_clear(GPIO_PIN_LED);

    // Initialize the DHT sensor
    dht_params_t dht_params;
    // Configure the DHT parameters accordingly
    dht_params.pin = DHT_PIN;
    dht_params.type = DHT_11;

    dht_init(&dev, &dht_params);

    while (1) {
        // Read temperature and humidity
        res = dht_read(&dev, &temperature, &humidity);
        if (res == DHT_OK) { // Check if read was successful
            if (temperature > TEMP_THRESHOLD) {
                // Turn on the LED
                gpio_set(GPIO_PIN_LED);
            } else {
                // Turn off the LED
                gpio_clear(GPIO_PIN_LED);
            }
        } else {
            // Print an error message if reading DHT failed
            printf("Error reading DHT11 sensor\n");
        }
        
        // Wait for 5 seconds before the next read
        xtimer_sleep(5);
    }
    return 0;
}