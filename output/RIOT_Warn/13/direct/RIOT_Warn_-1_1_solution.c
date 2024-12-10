#include <ztimer.h>
#include <dht.h>
#include <stdio.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(PORT_A, 10)  // Example pin where DHT11 is connected
#define LED_PIN GPIO_PIN(PORT_B, 0)   // Example pin where LED is connected
#define TEMP_THRESHOLD 30             // Temperature threshold for LED warning

static dht_t dev;

int main(void) {
    // Set the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }
    
    while (1) {
        int16_t temp, hum;
        
        // Read values from the DHT11 sensor
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Failed to read from DHT11 sensor");
        } else {
            // Print the temperature and humidity
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            
            // Check if the temperature exceeds the threshold
            if (temp / 10 >= TEMP_THRESHOLD) {
                // Turn on the LED
                gpio_set(LED_PIN);
                puts("Temperature exceeded threshold - LED ON");
            } else {
                // Turn off the LED
                gpio_clear(LED_PIN);
                puts("Temperature below threshold - LED OFF");
            }
        }
        
        // Wait for 5 seconds
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    
    return 0;
}