#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_A, 8)  // Example pin, set according to your setup
#define LED_PIN GPIO_PIN(PORT_B, 0) // Example pin, set according to your setup
#define TEMP_THRESHOLD 30           // Temperature threshold in Celsius
#define DELAY 5                  // Delay time in seconds

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_t dev;
    int16_t temperature, humidity;
    
    // Initialize the DHT sensor
    if (dht_init(&dev, GPIO_PIN(PORT_A, 8), DHT11) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return 1;
    }

    while (1) {
        // Read data from the sensor
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Error reading from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
            // Check if the temperature exceeds the threshold
            if (temperature / 10 > TEMP_THRESHOLD) {
                // Turn on the LED
                gpio_set(LED_PIN);
            } else {
                // Turn off the LED
                gpio_clear(LED_PIN);
            }
        }
        // Wait for 5 seconds
        xtimer_sleep(DELAY);
    }
    return 0;
}