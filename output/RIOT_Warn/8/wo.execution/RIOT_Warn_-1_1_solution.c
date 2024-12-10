// This code demonstrates how to read from a DHT11 sensor on an ESP32 using RIOT-OS.
// It periodically reads the temperature and humidity, and if the temperature is above a set threshold,
// it lights up an LED for a visual warning.

// Required libraries for RIOT-OS
#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 8)  // Define the pin where the DHT11 is connected.
#define LED_PIN GPIO_PIN(PORT_B, 10) // Define the pin where the LED is connected.
#define TEMP_THRESHOLD 30           // Define the temperature threshold in degrees Celsius.
#define SLEEP_INTERVAL (5U * US_PER_SEC) // 5 seconds interval between each sensor read.

static dht_t dev;

int main(void)
{
    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != 0) {
        puts("DHT initialization failed!");
        return 1;
    }

    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN); // Make sure the LED is off initially.

    int16_t temp, hum;

    while (1) {
        // Read temperature and humidity from the DHT11 sensor
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            
            // Check if the temperature exceeds the threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                printf("Warning: Temperature threshold exceeded!\n");
                gpio_set(LED_PIN); // Turn on the LED
            } else {
                gpio_clear(LED_PIN); // Turn off the LED
            }
	extimer_sleep(SLEEP_INTERVAL);  // Sleep for the specified interval
        } else {
            puts("Failed to read from DHT sensor!");
        }
        xtimer_sleep(SLEEP_INTERVAL);  // Sleep for the specified interval
    }
    return 0;
}