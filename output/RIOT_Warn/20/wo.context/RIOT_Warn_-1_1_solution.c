#include <stdio.h>
#include <xtimer.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN         GPIO_PIN(PORT_A, 6) // Pin to connect DHT11 data
#define LED_PIN         GPIO_PIN(PORT_B, 0) // Pin to connect LED
#define TEMP_THRESHOLD  25   // Temperature threshold in °C

int main(void)
{
    dht_t dev;
    dht_init(&dev, DHT_PIN);
    
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);  // Turn off LED initially

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);

            // Check if temperature exceeds threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        } else {
            printf("Failed to read data from DHT sensor\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}