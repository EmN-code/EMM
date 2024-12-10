#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN         GPIO_PIN(PORT_A, 6)   // DHT11 connected to pin A6
#define LED_PIN         GPIO_PIN(PORT_B, 5)   // LED connected to pin B5
#define TEMP_THRESHOLD  30                    // Threshold temperature to trigger LED
#define INTERVAL        5                     // Sensor reading interval in seconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize DHT11 sensor
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        printf("Error initializing DHT11 sensor\n");
        return 1;
    }

    // Configure LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a loop to read temperature every 5 seconds
    while(1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temp: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            
            // Check if temperature exceeds threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        }
        else {
            printf("Error reading DHT11 sensor\n");
        }

        // Wait for the next reading
        xtimer_sleep(INTERVAL);
    }

    return 0;
}