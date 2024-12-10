
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define DHT11_PIN GPIO_PIN(0, 2)  // GPIO 2 on port 0

// Function to read DHT11 sensor
int read_dht11(int *humidity, int *temperature) {
    // Placeholder for DHT11 reading logic
    // This should include the timing and bit-banging logic to read the sensor
    // For simplicity, we'll just simulate a reading here
    *humidity = 45;  // Simulated humidity value
    *temperature = 22;  // Simulated temperature value
    return 0;  // Return 0 on success
}

int main(void) {
    int humidity, temperature;

    // Initialize GPIO pin for DHT11
    gpio_init(DHT11_PIN, GPIO_OUT);

    while (1) {
        // Read DHT11 sensor
        if (read_dht11(&humidity, &temperature) == 0) {
            printf("Humidity: %d %%\n", humidity);
            printf("Temperature: %d C\n", temperature);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
