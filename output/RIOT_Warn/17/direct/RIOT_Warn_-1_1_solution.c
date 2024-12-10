// Including necessary header files for RIOT and peripherals in descending alphabetical order
#include "xtimer.h"     // For timers and delays
#include "timex.h"      // For time structures
#include "periph/gpio.h" // For GPIO functions
#include "dht.h"        // For DHT sensor functions

#define DHT_PIN GPIO_PIN(PORT_A, 2)  // GPIO pin where DHT11 is connected
#define LED_PIN GPIO_PIN(PORT_B, 5)  // GPIO pin where LED is connected
#define TEMP_THRESHOLD 30            // Temperature threshold in degree Celsius
#define SLEEP_INTERVAL (5 * US_PER_SEC) // Sleep interval in microseconds

void read_and_check_temperature(void) {
    dht_t dev;
    int16_t temperature, humidity;
    
    // Initialize DHT sensor
    if (dht_init(&dev, DHT_MODEL_DHT11) != 0) {
        puts("DHT initialization failed!");
        return;
    }

    while (1) {
        // Read data from the sensor
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            puts("Error reading DHT sensor!");
        } else {
            // Convert temperature to degree Celsius
            temperature /= 10;

            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity / 10);

            // Check if the temperature exceeds the threshold
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                puts("Warning: Temperature exceeded threshold!");
            } else {
                gpio_clear(LED_PIN);
            }
        }
        
        // Wait for the defined sleep interval
        xtimer_sleep(SLEEP_INTERVAL);
    }
}

int main(void) {
    // Initialize GPIO pin for the LED as output
    gpio_init(LED_PIN, GPIO_OUT);
    
    puts("Starting DHT11 Sensor and LED Monitoring");
    
    // Start reading and monitoring temperature
    read_and_check_temperature();

    // Should never be reached
    return 0;
}
