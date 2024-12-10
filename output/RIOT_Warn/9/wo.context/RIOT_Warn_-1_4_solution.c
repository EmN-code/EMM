#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_A, 15)      // Assuming the DHT is connected to PORT A, pin 15
#define LED_PIN GPIO_PIN(PORT_B, 12)      // Assuming the LED is connected to PORT B, pin 12
#define TEMP_THRESHOLD 30                 // Temperature threshold in degree Celsius

static dht_t dev;

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT sensor.\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;

        // Trigger DHT11 sensor
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read data from DHT sensor.\n");
        } else {
            printf("Temperature: %d.%d°C\n", temp / 10, temp % 10);
            printf("Humidity: %d.%d%%\n", hum / 10, hum % 10);

            if (temp > TEMP_THRESHOLD * 10) {  // Convert TEMP_THRESHOLD to deci-degrees
                gpio_set(LED_PIN);   // Turn on the LED
            } else {
                gpio_clear(LED_PIN); // Turn off the LED
            }
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}