#include <xtimer.h>  // For timer management
#include <timex.h>   // For time management
#include <stdio.h>   // For standard I/O operations
#include <periph/gpio.h>  // For GPIO operations
#include <dht.h>  // DHT sensor interface

#define DHT_PIN GPIO_PIN(0, 4)  // The GPIO pin where the DHT sensor's data line is connected
#define LED_PIN GPIO_PIN(0, 15)  // The GPIO pin where the LED is connected
#define TEMP_THRESHOLD 30  // Temperature threshold for LED warning
#define DHT_TYPE DHT11  // Type of DHT sensor
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds interval in microseconds

// Function declarations
void read_dht(void);

// Main function
int main(void) {
    // Initialize GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize DHT sensor
    dht_t dev;
    dht_init(&dev, DHT_PIN);
    
    while (1) {
        read_dht();  // Read the DHT sensor
        xtimer_sleep(5);  // Sleep 5 seconds
    }
    return 0;
}

void read_dht(void) {
    int16_t temp, hum;
    dht_t dev;

    // Read values from the DHT sensor
    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);

        // Check if the temperature exceeds the defined threshold
        if (temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);  // Turn on LED if temperature exceeds threshold
        } else {
            gpio_clear(LED_PIN);  // Turn off LED otherwise
        }
    } else {
        printf("Failed to read from DHT sensor\n");
    }
}
