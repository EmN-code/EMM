#include <xtimer.h>  // Timer library for periodic execution
#include <inttypes.h>  // Integer type definitions
#include <stdio.h>     // Standard input/output definitions
#include <periph/gpio.h>  // GPIO pin control

// Define GPIO pins for DHT11 and LED
#define DHT11_PIN GPIO_PIN(PORT_A, 0)  // Specify the correct port and pin for the DHT11
#define LED_PIN GPIO_PIN(PORT_B, 1)    // Specify the correct port and pin for the LED

// Temperature threshold for warning
#define TEMPERATURE_THRESHOLD 30  // Example threshold value

// Function to simulate reading temperature from DHT11
int read_dht11_temperature(void) {
    // Simulate reading the temperature (In real code, replace with actual sensor reading)
    return 25;  // Return a dummy temperature value for simulation
}

int main(void) {
    // Initialize GPIO pins
    gpio_init(DHT11_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);

    printf("DHT11 Temperature Monitoring Program\n");

    while (1) {
        int temperature = read_dht11_temperature();
        printf("Current temperature: %d°C\n", temperature);

        if (temperature > TEMPERATURE_THRESHOLD) {
            gpio_set(LED_PIN);  // Turn on LED if temperature exceeds threshold
            printf("Temperature exceeds threshold! LED is ON.\n");
        } else {
            gpio_clear(LED_PIN);  // Turn off LED otherwise
            printf("Temperature is below threshold. LED is OFF.\n");
        }

        xtimer_sleep(5);  // Sleep for 5 seconds before reading again
    }
    return 0;
}