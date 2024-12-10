#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(PORT_A, 0)  // Define the GPIO pin for DHT11
#define LED_PIN GPIO_PIN(PORT_B, 0)  // Define the GPIO pin for the LED
#define TEMP_THRESHOLD 30            // Temperature threshold in Celsius
#define READ_INTERVAL (5 * XTIMER_SEC) // Set the read interval to 5 seconds

void init_pins(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);
    // Proper initialization of the DHT_PIN for the DHT11 sensor
    // DHT11 sensor-specific initialization should be correctly placed here
}

int read_dht11(int *temperature) {
    // Mocked function to simulate reading from DHT11
    // Replace this with actual library function as appropriate
    *temperature = 25 + (rand() % 15);  // Random temperature for simulation
    return 0;  // Success
}

int main(void) {
    int temperature;
    init_pins();

    while (1) {
        if (read_dht11(&temperature) == 0) {
            printf("Current Temperature: %d°C\n", temperature);
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
                printf("Warning: Temperature exceeded the threshold!\n");
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        xtimer_sleep(READ_INTERVAL);
    }

    return 0;
}