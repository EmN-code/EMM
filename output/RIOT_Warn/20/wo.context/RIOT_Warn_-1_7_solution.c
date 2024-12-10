// Include headers in alphabetical order by their first letter
#include <xtimer.h>
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0) // Assuming the DHT11 is connected to pin A0, change if needed
#define LED_PIN GPIO_PIN(PORT_B, 0) // Assuming the LED is connected to pin B0, change if needed
#define TEMP_THRESHOLD 30 // Temperature warning threshold in degrees Celsius
#define SLEEP_INTERVAL (5 * US_PER_SEC) // 5 seconds interval

int main(void) {
    // Initialize the DHT sensor
    if (dht_init(&DHT_PIN) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    int16_t temperature = 0;
    int16_t humidity = 0;

    while (1) {
        // Read values from the DHT11 sensor
        if (dht_read(DHT_PIN, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

            // Check if the temperature exceeds the threshold
            if (temperature > TEMP_THRESHOLD) {
                // Turn on the LED
                gpio_set(LED_PIN);
            } else {
                // Turn off the LED
                gpio_clear(LED_PIN);
            }
        } else {
            puts("Failed to read from DHT sensor");
        }

        // Sleep for the defined interval
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }

    return 0;
}