// Include necessary header files for RIOT
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include "thread.h"
#include <stdio.h>

// Define the DHT11 parameter
#define DHT11_PIN GPIO_PIN(0, 10)  // Adjust pin as needed
#define DHT11_TYPE DHT11

// Define the LED warning parameter
#define LED_PIN GPIO_PIN(0, 12)  // Adjust pin as needed
#define TEMP_THRESHOLD 30  // Temperature threshold in Celsius
#define READ_INTERVAL (5 * US_PER_SEC)  // Read every 5 seconds

// DHT device
static dht_t dev;

// Function prototype
void read_sensor(void);

// Main function
int main(void) {
    // Initialize the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the DHT11 sensor
    dht_params_t params;
    params.pin = DHT11_PIN;
    params.type = DHT11_TYPE;
    if (dht_init(&dev, &params) != 0) {
        printf("DHT11 initialization failed\n");
        return 1;
    }

    printf("DHT11 sensor initialized\n");

    // Event loop
    while (1) {
        read_sensor();
        xtimer_sleep(5);  // Sleep for 5 seconds before next read
    }

    return 0;
}

// Function to read the sensor and control the LED
void read_sensor(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) != 0) {
        printf("DHT11 read failed\n");
        return;
    }

    printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);

    // Check if temperature exceeds threshold
    if (temp > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);  // Turn on the LED
        printf("Warning: Temperature exceeds threshold!\n");
    } else {
        gpio_clear(LED_PIN);  // Turn off the LED
    }
}