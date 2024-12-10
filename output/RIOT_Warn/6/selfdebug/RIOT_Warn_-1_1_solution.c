#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN  GPIO_PIN(PORT_A, 5) // Define the GPIOA pin 5 for DHT11
#define LED_PIN  GPIO_PIN(PORT_B, 0) // Define the GPIOB pin 0 for the LED
#define THRESHOLD_TEMP  30 // Temperature threshold in Celsius
#define INTERVAL (5 * 1000000U) // 5 seconds expressed in microseconds

dht_t dht_dev;

int main(void) {
    // Initialize the DHT11 sensor
    dht_params_t dht_params;
    dht_params.pin = DHT_PIN;
    dht_params.type = DHT11;
    if (dht_init(&dht_dev, &dht_params) != 0) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN); // Ensure LED is initially off

    // Main loop
    while (1) {
        int16_t temp, hum;
        // Read temperature and humidity
        if (dht_read(&dht_dev, &temp, &hum) != 0) {
            printf("Failed to read DHT11\n");
        } else {
            printf("Temperature: %d.%dC, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            // Check if temperature exceeds threshold
            if (temp / 10 > THRESHOLD_TEMP) {
                gpio_set(LED_PIN); // Turn on LED
            } else {
                gpio_clear(LED_PIN); // Turn off LED
            }
        }
        xtimer_usleep(INTERVAL); // Wait for defined interval
    }
    return 0;
}