#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define DHT11_PORT        GPIO_PIN(0, 1) // Example port, adjust as needed
#define LED_PORT          GPIO_PIN(0, 2) // Example port, adjust as needed
#define TEMP_THRESHOLD    30             // Temperature threshold in degrees Celsius

// Function to simulate reading temperature from DHT11
int read_dht11_temperature(void) {
    // This is a mock function. Implement real reading here.
    return 25 + rand() % 20; // Random temperature for testing
}

int main(void) {
    gpio_init(DHT11_PORT, GPIO_IN);
    gpio_init(LED_PORT, GPIO_OUT);

    while (1) {
        int temperature = read_dht11_temperature();
        printf("Current Temperature: %d°C\n", temperature);

        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PORT);
            printf("Warning: Temperature exceeds threshold!\n");
        } else {
            gpio_clear(LED_PORT);
        }
        
        xtimer_sleep(5); // Sleep for 5 seconds
    }
    return 0;
}