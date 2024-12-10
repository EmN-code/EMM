#include <xtimer.h>  // For timer functionalities
#include <stdio.h>   // Standard library for input/output
#include <dht.h>     // DHT sensor library
#include <periph/gpio.h>  // For GPIO operations

// Define GPIO pins
#define DHT11_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_B, 0)

#define TEMP_THRESHOLD 25  // Temperature threshold for LED warning

int main(void) {
    // Initialize DHT sensor
    if (dht_init(DHT11_PIN) != DHT_OK) {
        printf("Error: Unable to initialize DHT sensor\n");
        return 1;
    }
    
    // Configure LED Pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        int16_t temp, hum;
        if (dht_read(DHT11_PIN, &temp, &hum) != DHT_OK) {
            printf("Error: Unable to read data from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5);
    }
    return 0;
}