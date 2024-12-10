#include <xtimer.h> // Includes xtimer for delays
#include <periph/gpio.h> // Includes peripheral GPIO functions

#define TEMP_THRESHOLD 30 // Define the temperature threshold
#define DHT11_PIN GPIO_PIN(PORT_A, 0) // Define the GPIO pin for DHT11
#define LED_PIN GPIO_PIN(PORT_A, 1) // Define the GPIO pin for LED

static int read_temperature(void) {
    // Dummy function to read temperature from the DHT11 sensor
    // This should be replaced with actual read code for DHT11
    return 28; // placeholder for temperature value
}

int main(void) {
    // Initialize GPIO
    gpio_init(DHT11_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);
    
    while (1) {
        int temperature = read_temperature(); // Read the temperature

        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN); // Turn on LED if temperature is above threshold
        } else {
            gpio_clear(LED_PIN); // Turn off LED if temperature is below threshold
        }
        
        xtimer_sleep(5); // Wait for 5 seconds
    }
    return 0;
}