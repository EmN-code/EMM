#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PA,5) // Adjust pin as necessary 
#define LED_PIN GPIO_PIN(PA,10) // Adjust pin as necessary
#define TEMP_THRESHOLD 30
#define DELAY (5U * 1000000U) // 5 seconds converted to microseconds

// Global variable for the DHT device
static dht_t dht_dev;

// Function to initialize the DHT11 sensor
void init_dht(void) {
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;
    dht_init(&dht_dev, &params);
}

// Main function
int main(void) {
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    // Initialize DHT sensor
    init_dht();
    
    int16_t temp, hum;
    
    while (1) {
        // Read temperature and humidity
        if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
            printf("Temp: %d.%d°C, Humidity: %d.%d %%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            // Check if temperature exceeds threshold
            if ((temp / 10) > TEMP_THRESHOLD) {
                // Turn on LED if temperature is above threshold
                gpio_set(LED_PIN);
            } else {
                // Turn off LED otherwise
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        // Wait for 5 seconds
        xtimer_usleep(DELAY);
    }
    return 0;
}
