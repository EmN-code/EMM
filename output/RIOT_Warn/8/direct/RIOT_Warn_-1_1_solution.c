#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 4)  // Define the pin where the DHT11 data line is connected
#define LED_PIN GPIO_PIN(0, 2)  // Define the pin where the LED is connected
#define TEMP_THRESHOLD 30       // Define the temperature threshold in Celsius

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize DHT sensor
    dht_t dev;
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        puts("DHT initialization failed");
        return 1;
    }

    int16_t temp, hum;

    while (1) {
        // Read the DHT11 sensor values
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Failed to read DHT sensor");
        }
        else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            // Check if the temperature exceeds the threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                puts("Temperature exceeded threshold! LED ON");
            }
            else {
                gpio_clear(LED_PIN);
                puts("Temperature is normal. LED OFF");
            }
        }

        // Wait 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}