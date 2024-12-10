
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <periph/gpio.h>
#include <xtimer.h>

#define DHT_PIN            GPIO_PIN(0, 18)  // Example GPIO pin for DHT11
#define LED_PIN            GPIO_PIN(0, 2)   // Example GPIO pin for LED
#define TEMP_THRESHOLD     30               // Temperature threshold in Celsius
#define SLEEP_INTERVAL     5                // Sleep interval in seconds

void read_dht11(int16_t *temp, int16_t *hum) {
    // Simulated DHT11 reading function
    // Replace this with actual DHT11 reading logic
    *temp = 25;  // Example temperature reading
    *hum = 50;   // Example humidity reading
}

int main(void) {
    int16_t temperature, humidity;

    // Initialize GPIO pins
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_init(DHT_PIN, GPIO_IN);

    while (1) {
        // Read DHT11 sensor data
        read_dht11(&temperature, &humidity);

        // Print temperature and humidity
        printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

        // Check if temperature exceeds the threshold
        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);  // Turn on the LED
        } else {
            gpio_clear(LED_PIN);  // Turn off the LED
        }

        // Sleep for 5 seconds
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}
