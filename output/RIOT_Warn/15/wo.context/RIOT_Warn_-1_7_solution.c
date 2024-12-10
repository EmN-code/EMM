#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <net/network.h>

#define DHT_GPIO_PIN GPIO_PIN(PORT_A, 0)  // Correctly define the DHT11 data pin
#define LED_GPIO_PIN GPIO_PIN(PORT_B, 1)  // Correctly define the LED pin
#define TEMP_THRESHOLD 30  // Temperature threshold for LED warning

// Stub for initializing DHT11 sensor
int dht_init(gpio_t pin);

// Function to read temperature from DHT sensor (stub)
int dht_read_temperature(int *temperature);

int main(void) {
    // Initialize DHT11 sensor
    if (dht_init(DHT_GPIO_PIN) != 0) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }
    
    // Initialize LED pin
    gpio_init(LED_GPIO_PIN, GPIO_OUT);

    while (1) {
        int temperature;

        // Read temperature from DHT11 sensor
        if (dht_read_temperature(&temperature) == 0) {
            printf("Temperature: %d\n", temperature);

            // Check if temperature exceeds threshold
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_GPIO_PIN);
                printf("Warning: Temperature exceeds threshold!\n");
            } else {
                gpio_clear(LED_GPIO_PIN);
            }
        } else {
            printf("Error reading temperature from DHT11\n");
        }

        xtimer_sleep(5);  // Wait for 5 seconds before reading again
    }

    return 0;
}