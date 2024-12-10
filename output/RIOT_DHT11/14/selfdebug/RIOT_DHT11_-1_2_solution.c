// Description: This program reads data from a DHT11 sensor using the ESP32 every 5 seconds. The DHT11 is connected to GPIO 2. The program initializes the sensor, reads temperature and humidity values, and prints them to the console.

// Required imports for ESP32 and DHT11 with RIOT-OS
#include "timex.h"  // For time-related functions
#include "periph/gpio.h"  // For GPIO functions
#include "dht.h"  // For DHT sensor functions

// Define some constants
#define DHT_PIN GPIO_PIN(0, 2)  // The GPIO pin where DHT11 is connected
#define INTERVAL (5U)  // Interval for sensor reading in seconds

// Function to read data from DHT11 sensor
void read_dht_sensor(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT11, DHT_PIN) != DHT_OK) {
        printf("Error: Unable to initialize DHT11 sensor.\n");
        return;
    }

    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d, Humidity: %d\n", temp / 10, hum / 10);  // Dividing by 10 to convert to correct format
    } else {
        printf("Error: Failed to read from DHT11 sensor.\n");
    }
}

// Main function
int main(void) {
    printf("DHT11 Sensor reading started...\n");
    while (1) {
        read_dht_sensor();
        xtimer_sleep(INTERVAL);
    }
    return 0;
}