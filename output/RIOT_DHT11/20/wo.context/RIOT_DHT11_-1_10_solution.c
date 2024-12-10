#include "xtimer.h"  // For timer functionality
#include "periph/gpio.h"  // For GPIO functions
#include "dht.h"  // DHT sensor functions

// Define the parameters for the DHT sensor
#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2 on ESP32
#define DHT_TYPE DHT11  // We are using the DHT11 sensor

// Function to initialize the DHT sensor
void init_dht(dht_t *dev) {
    // Initialize the DHT device with specified pin and type
    dev->pin = DHT_PIN;
    dev->type = DHT_TYPE;
}

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT sensor
    init_dht(&dev);

    while (1) {
        // Read values from DHT sensor
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}