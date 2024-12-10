#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include "thread.h"

// Define the GPIO pins for DHT11 and LED
#define DHT11_PIN GPIO_PIN(PORT_A, 10)
#define LED_PIN GPIO_PIN(PORT_A, 5)

#define TEMP_THRESHOLD 30  // Temperature threshold in degree Celsius

void read_dht11(void)
{
    int16_t temperature, humidity;
    dht_t dev;
    dht_params_t params = { DHT11_PIN };

    // Initialize the DHT sensor
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return;
    }

    // Read the values from DHT sensor
    if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
        printf("Temperature: %d °C, Humidity: %d %%\n", temperature / 10, humidity / 10);
        
        // Check if the temperature exceeds the threshold
        if ((temperature / 10) > TEMP_THRESHOLD) {
            // Turn on the LED
            gpio_set(LED_PIN);
        } else {
            // Turn off the LED
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Failed to read from DHT11\n");
    }
}

int main(void)
{
    // Initialize the GPIO for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Infinite loop to read the sensor every 5 seconds
    while (1) {
        read_dht11();
        xtimer_sleep(5);
    }

    return 0;
}