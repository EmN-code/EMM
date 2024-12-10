#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN          GPIO_PIN(PORT_A, 9)   // Define the GPIO pin for the DHT11 sensor
#define DHT_TYPE         DHT_TYPE_DHT11       // Define the sensor type, DHT11
#define LED_PIN          GPIO_PIN(PORT_B, 0)  // Define the GPIO pin for the LED
#define TEMP_THRESHOLD   30                   // Threshold temperature for turning on the LED
#define SLEEP_DURATION   (5 * US_PER_SEC)     // Sleep duration, 5 seconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT_TYPE) != DHT_OK) {
        printf("Failed to initialize DHT sensor.\n");
        return -1;
    }

    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        printf("Failed to initialize LED pin.\n");
        return -1;
    }

    while (1) {
        // Read values from the DHT sensor
        if (dht_measure(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            
            // Check temperature threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("LED turned ON\n");
            } else {
                gpio_clear(LED_PIN);
                printf("LED turned OFF\n");
            }
        }
        
        // Sleep for the specified duration
        xtimer_usleep(SLEEP_DURATION);
    }

    return 0;
}