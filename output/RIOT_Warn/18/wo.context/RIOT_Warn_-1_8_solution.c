#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <periph/i2c.h>

#define DHT11_PIN GPIO_PIN(0, 5)  // Define GPIO0 pin 5 for DHT11 data
#define LED_PIN GPIO_PIN(0, 2)   // Define GPIO0 pin 2 for the LED
#define TEMPERATURE_THRESHOLD 25 // Define temperature threshold, e.g., 25°C

int read_dht11(int pin, int *temperature) {
    // Simulated function to read temperature from DHT11 sensor
    // In a real scenario, you would interface with the sensor's library
    *temperature = 26; // Dummy value for demonstration purposes
    return 0; // Return 0 on success
}

void check_temperature_and_control_led(void) {
    int temperature;
    if (read_dht11(DHT11_PIN, &temperature) == 0) {
        printf("Temperature: %d°C\n", temperature);
        if (temperature > TEMPERATURE_THRESHOLD) {
            gpio_set(LED_PIN);
            printf("Temperature exceeds threshold. LED turned on.\n");
        } else {
            gpio_clear(LED_PIN);
            printf("Temperature is below threshold. LED turned off.\n");
        }
    } else {
        printf("Error reading from DHT11 sensor.\n");
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    while (1) {
        check_temperature_and_control_led();
        xtimer_sleep(5); // Wait for 5 seconds
    }
    return 0;
}