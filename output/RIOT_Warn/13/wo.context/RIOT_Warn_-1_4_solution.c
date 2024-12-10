#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define DHT11_PIN GPIO_PIN(PORT_A, 0) // Assuming DHT11 is connected to GPIO0
#define LED_PIN GPIO_PIN(PORT_B, 2)   // Assuming LED is connected to GPIO2
#define TEMP_THRESHOLD 30             // Temperature threshold for warning

int main(void) {
    printf("Initializing DHT11 and LED GPIO\n");
    
    // Initialize DHT11 pin as input
    if (gpio_init(DHT11_PIN, GPIO_IN) < 0) {
        printf("Error initializing GPIO for DHT11\n");
        return 1;
    }

    // Initialize LED pin as output
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Error initializing GPIO for LED\n");
        return 1;
    }

    while (1) {
        int temp = read_dht11_temperature(DHT11_PIN); // Assuming a function to read temp

        if (temp > TEMP_THRESHOLD) {
            printf("Temperature is %d, which exceeds threshold, turning on LED\n", temp);
            gpio_set(LED_PIN);
        } else {
            printf("Temperature is %d, within safe range, turning off LED\n", temp);
            gpio_clear(LED_PIN);
        }
        xtimer_sleep(5); // Sleep for 5 seconds
    }
    return 0;
}

// Mock function for reading temperature from DHT11
int read_dht11_temperature(gpio_t pin) {
    // Here should be the code that interfaces with the DHT11 sensor
    // For simulation, a constant value or a random value may be used
    return 25;  // Returning a dummy value
}