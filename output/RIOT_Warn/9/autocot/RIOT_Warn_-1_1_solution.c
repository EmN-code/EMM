// Include header files in descending alphabetical order
#include "xtimer.h" // For the timer
#include "thread.h" // For threading support
#include "periph/gpio.h" // For GPIO access
#include "dht.h" // For DHT sensor
#include <stdio.h> // Standard I/O for printf

#define LED_PIN GPIO_PIN(PORT_A, 5) // Define the GPIO pin where LED is connected
#define DHT_PIN GPIO_PIN(PORT_B, 7) // Define the GPIO pin where DHT11 is connected
#define TEMP_THRESHOLD 30 // Temperature threshold in Celsius
#define INTERVAL 5 // Interval in seconds

// Declare a dht device
static dht_t dht_dev;

int main(void) {
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing LED pin\n");
        return 1;
    }
    // Initialize the DHT11 sensor
    dht_params_t dht_params;
    dht_params.pin = DHT_PIN;
    dht_params.type = DHT11;
    if (dht_init(&dht_dev, &dht_params) != 0) {
        printf("Error initializing DHT sensor\n");
        return 1;
    }

    while (1) {
        int16_t temperature, humidity;

        // Read temperature and humidity
        if (dht_read(&dht_dev, &temperature, &humidity) != 0) {
            printf("Failed to read from DHT sensor\n");
            xtimer_sleep(INTERVAL);
            continue;
        }

        // Convert temperature to Celsius (as it is returned as an integer with one decimal place)
        temperature /= 10;

        printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);

        // Check temperature and turn on/off LED accordingly
        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
            printf("Warning: Temperature exceeded threshold!\n");
        } else {
            gpio_clear(LED_PIN);
        }

        // Sleep for the defined interval
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
