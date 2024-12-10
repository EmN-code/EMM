#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_A, 0) // Define the correct GPIO pin for your setup
#define LED_PIN GPIO_PIN(PORT_A, 1) // Define the correct GPIO pin for your setup
#define TEMP_THRESHOLD 30 // Temperature threshold
#define DELAY 5U

int main(void) {
    // Initialize DHT sensor and LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    dht_t dev;
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT_TYPE_DHT11;
    
    // Infinite loop to continuously read temperature and humidity
    while (1) {
        int16_t temperature, humidity;
        if (dht_read(&dev, &params, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d m°C, Humidity: %d %%RH\n", temperature, humidity);
            
            // Check if temperature exceeds threshold
            if (temperature > (TEMP_THRESHOLD * 100)) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        // Wait for 5 seconds
        xtimer_sleep(DELAY);
    }
    return 0;
}
