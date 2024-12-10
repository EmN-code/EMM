#include <string.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <dht.h>
#include <stdio.h>

#define DHT_PIN GPIO_PIN(0, 10) // Define the GPIO pin where DHT11 is connected
#define LED_PIN GPIO_PIN(0, 11) // Define the GPIO pin for the LED
#define TEMP_THRESHOLD 30 // Temperature threshold in Celsius

// Placeholder for reading from DHT11.
int read_temperature(int *temperature) {
    int attempts = 0;
    while (attempts < 3) { // Attempt to read up to 3 times
        int temp = 25; // Replace with actual DHT read function
        if (temp >= 0) { // Replace with actual check for valid data
            *temperature = temp;
            return 0;
        }
        attempts++;
    }
    return -1;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        int temperature = 0;
        if (read_temperature(&temperature) == 0) {
            printf("Current temperature: %d°C\n", temperature);

            // Check if the temperature exceeds the threshold
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN); // Turn on the LED
                printf("Warning: Temperature threshold exceeded!\n");
            } else {
                gpio_clear(LED_PIN); // Turn off the LED
            }
        } else {
            printf("Failed to read temperature from DHT11 sensor\n");
        }

        xtimer_sleep(5); // Wait for 5 seconds before next read
    }

    return 0;
}