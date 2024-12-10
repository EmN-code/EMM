#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_A, 10) // Example pin, should be adjusted per your setup
#define LED_PIN GPIO_PIN(PORT_B, 0)   // Example pin, should be adjusted per your setup
#define DHT_TYPE DHT11
#define TEMP_THRESHOLD 30
#define READ_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    
    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_TYPE, DHT_PIN) != 0) {
        printf("DHT initialization failed\n");
        return 1;
    }
    
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("LED GPIO initialization failed\n");
        return 1;
    }

    // Continuously read from the DHT sensor every 5 seconds
    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
            
            // Check if the temperature exceeds the threshold
            if (temperature > (TEMP_THRESHOLD * 10)) {
                gpio_set(LED_PIN); // Turn on LED
            } else {
                gpio_clear(LED_PIN); // Turn off LED
            }
        }
        
        // Wait for the next cycle
        xtimer_usleep(READ_INTERVAL);
    }

    return 0;
}
